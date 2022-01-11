#ifndef __DEVICESOCKET_H__
#define __DEVICESOCKET_H__

#include <QTcpSocket>
#include <QWaitCondition>
#include <QMutex>

class DeviceSocket : public QTcpSocket{

public:
  explicit DeviceSocket(QObject* parent = nullptr);

  qint32 subThreadRecvData(quint8* buffer, quint32 bufferSize);

  ~DeviceSocket();

protected slots:
  void onReadyRead();
  void quitNotify();

private:
  QMutex m_mutex;
  QWaitCondition m_recvDataCond; //该条件变量用于通知解码线程数据到来

  //标志
  bool m_recvData;
  bool m_quit;

  //数据缓存
  quint8* m_buffer;
  qint32 m_bufferSize;
  qint32 m_dataSize;
};

#endif // __DEVICESOCKET_H__
