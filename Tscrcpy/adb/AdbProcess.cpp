#include <QDebug>
#include "AdbProcess.h"

AdbProcess::AdbProcess(QObject* parent) : QProcess(parent){

  initSignals();

  connect(this, SIGNAL(started()), this, SLOT(onStarted()));
  //connect(this, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onErrorOccurred(QProcess::PRocessError)));
  connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)));
  connect(this, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));
  connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));
}

void AdbProcess::initSignals(){

  /*connect(this, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
      [=](int exitCode, QProcess::ExitStatus exitStatus){

    qDebug() << exitCode << exitStatus;
  });*/

  connect(this, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error){

    qDebug() << error;
  });
}

void AdbProcess::onStarted(){

  qDebug() << "started..";
}

void AdbProcess::onErrorOccurred(QProcess::ProcessError error){

  qDebug() << error;
}

void AdbProcess::onFinished(int exitCode, QProcess::ExitStatus exitStatus){

  qDebug() << exitCode << exitStatus;
}

void AdbProcess::onReadyReadStandardError(){

  qDebug() << readAllStandardError();
}

void AdbProcess::onReadyReadStandardOutput(){

  qDebug() << readAllStandardOutput();
}
