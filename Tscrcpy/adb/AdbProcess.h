#ifndef __ADBPROCESS_H__
#define __ADBPROCESS_H__

#include <QProcess>
#include <QStringList>
#include <QObject>

class AdbProcess : public QProcess{

  Q_OBJECT

public:
  enum ADB_EXEC_RESULT{

    AER_SUCCESS_START,              //启动成功
    AER_ERROR_START,                //启动失败
    AER_SUCCESS_EXEC,               //执行成功
    AER_ERROR_EXEC,                 //执行失败
    AER_ERROR_MISSING_BINARY        //未找到文件
  };

  AdbProcess(QObject* parent = nullptr);

  static const QString getAdbPath();

  void execute(const QString& serial, const QStringList& args);
  void push(const QString& serial, const QString& local, const QString& remote);
  void removeFile(const QString& serial, const QString& filePath);
  void reverse(const QString& serial, const QString& deviceSocketName, qint16 localPort);
  void removeReverse(const QString& serial, const QString& deviceSocketName);
  QStringList getDeviceSerialFromStdOut();
  QString getDeviceIpFromStdOut();
  QString getStanddOut();
  QString getErrorOut();

private:
  void initSignals();

signals:
  void adbProcessResult(ADB_EXEC_RESULT processResult);

protected slots:
  void onStarted();
  void onErrorOccurred(QProcess::ProcessError error);
  void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void onReadyReadStandardError();
  void onReadyReadStandardOutput();

private:
  static QString s_adbPath;
  QString m_standardOutput;
  QString m_errorOutput;
};

#endif // __ADBPROCESS_H__
