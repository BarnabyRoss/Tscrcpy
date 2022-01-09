#include <QFileInfo>
#include <QCoreApplication>
#include <QRegExp>
#include <QDebug>
#include "AdbProcess.h"

QString AdbProcess::s_adbPath = "";

AdbProcess::AdbProcess(QObject* parent) : QProcess(parent), m_standardOutput(""), m_errorOutput(""){

  initSignals();

  connect(this, SIGNAL(started()), this, SLOT(onStarted()));
  //connect(this, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onErrorOccurred(QProcess::PRocessError)));
  connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)));
  connect(this, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));
  connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));

  getAdbPath();
}

const QString AdbProcess::getAdbPath(){

  QString m_adbPath = "";
  if( m_adbPath.isEmpty() ){

    m_adbPath = QString::fromLocal8Bit(qgetenv("TSCRCPY_ADB_PATH"));
    QFileInfo fileInfo(m_adbPath);

    if( m_adbPath.isEmpty() || !fileInfo.isFile() ){

      m_adbPath = QCoreApplication::applicationDirPath() + "/adb";
    }
  }

  return m_adbPath;
}

void AdbProcess::execute(const QString& serial, const QStringList& args){

  QStringList adbArgs;
  if( !serial.isEmpty() ){

    adbArgs << "-s" << serial;
  }
  adbArgs << args;
  //adbArgs.join(" ");

  qDebug() << adbArgs;

  this->start(getAdbPath(), adbArgs);
}

void AdbProcess::push(const QString& serial, const QString& local, const QString& remote){

  QStringList args;
  args << "push" << local << remote;
  execute(serial, args);
}

void AdbProcess::removeFile(const QString& serial, const QString& filePath){

  QStringList args;
  args << "shell" << "rm" << filePath;

  execute(serial, args);
}

void AdbProcess::reverse(const QString& serial, const QString& deviceSocketName, qint16 localPort){

  QStringList args;
  args << "reverse";
  args << QString("localabstract:%1").arg(deviceSocketName);
  args << QString("tcp:%1").arg(localPort);

  execute(serial, args);
}

void AdbProcess::removeReverse(const QString& serial, const QString& deviceSocketName){

  QStringList args;
  args << "reverse" << "--remove";
  args << QString("localabstract:%1").arg(deviceSocketName);

  execute(serial, args);
}

QStringList AdbProcess::getDeviceSerialFromStdOut(){

  //"List of devices attached\r\n8LS8SOEQ99999999\tdevice"
  QStringList serials;
  QStringList deviceInfoList = m_standardOutput.split(QRegExp("\r\n|\n"), QString::SkipEmptyParts);
  for(QString deviceInfo : deviceInfoList){

    QStringList tmp = deviceInfo.split(QRegExp("\t"), QString::SkipEmptyParts);
    if( tmp.count() == 2 && (tmp[1].compare("device") == 0) ){

      serials << tmp[0];
    }
  }

  return serials;
}

void AdbProcess::initSignals(){

  /*connect(this, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
      [=](int exitCode, QProcess::ExitStatus exitStatus){

    qDebug() << exitCode << exitStatus;
  });*/

  connect(this, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error){

    if( error == QProcess::FailedToStart ){

      emit adbProcessResult(AER_ERROR_MISSING_BINARY);

    }else{

      emit adbProcessResult(AER_ERROR_START);
    }
    qDebug() << error;
  });
}

void AdbProcess::onStarted(){

  emit adbProcessResult(AER_SUCCESS_START);
  qDebug() << "started..";
}

void AdbProcess::onErrorOccurred(QProcess::ProcessError error){

  qDebug() << error;
}

void AdbProcess::onFinished(int exitCode, QProcess::ExitStatus exitStatus){

  if( exitStatus == QProcess::NormalExit && exitCode == 0 ){

    emit adbProcessResult(AER_SUCCESS_EXEC);

  }else{

    emit adbProcessResult(AER_ERROR_EXEC);
  }

  qDebug() << exitCode << exitStatus;
}

void AdbProcess::onReadyReadStandardError(){

  m_errorOutput = QString::fromLocal8Bit(readAllStandardError()).trimmed();
  qDebug() << m_errorOutput;
}

void AdbProcess::onReadyReadStandardOutput(){

  m_standardOutput = QString::fromLocal8Bit(readAllStandardOutput()).trimmed();
  qDebug() << m_standardOutput;
}
