#ifndef __SERVER_H__
#define __SERVER_H__

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTcpServer>
#include "AdbProcess.h"

class Server : public QObject{

  Q_OBJECT

  //状态机
  enum SERVER_START_STEP{

    SSS_NULL,
    SSS_PUSH,
    SSS_ENABLE_REVERSE,
    SSSS_EXECUTE_SERVER,
    SSS_RUNNING
  };

public:
  Server(QObject* parent = nullptr);

  bool start(const QString& serial, quint16 localPort, quint16 maxSize, quint32 bitRate);
  void stop();

  QString getServerPath();

signals:
  void serverStartResult(bool success); //server执行成功还是失败
  void connectToResult(bool success, const QString& deviceName, const QSize& size);

protected slots:
  void onAdbProcessResult(AdbProcess::ADB_EXEC_RESULT processResult);

private:
  bool startServerByStep();
  bool pushServer();
  bool enableTunnelReverse();
  bool disableTunnelReverse();
  bool removeServer();
  bool executeServer();

private:
  QString m_serial;
  quint16 m_localPort;
  quint16 m_maxSize;
  quint32 m_bitRate;

  SERVER_START_STEP m_serverStartStep;

  AdbProcess m_workProcess;
  AdbProcess m_serverProcess;
  QString m_serverPath;
  bool m_serverCopiedToDevice;
  bool m_enableReverse;

  QTcpServer m_serverSocket;
  QTcpSocket* m_deviceSocket;
};

#endif // __SERVER_H__
