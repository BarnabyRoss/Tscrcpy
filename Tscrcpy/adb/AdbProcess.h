#ifndef __ADBPROCESS_H__
#define __ADBPROCESS_H__

#include <QProcess>
#include <QObject>

class AdbProcess : public QProcess{

  Q_OBJECT

public:
  AdbProcess(QObject* parent = nullptr);

private:
  void initSignals();

protected slots:
  void onStarted();
  void onErrorOccurred(QProcess::ProcessError error);
  void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void onReadyReadStandardError();
  void onReadyReadStandardOutput();
};

#endif // __ADBPROCESS_H__
