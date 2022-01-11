#include <QCoreApplication>
#include <QThread>
#include <QMutexLocker>
#include "DeviceSocket.h"

DeviceSocket::DeviceSocket(QObject* parent) : QTcpSocket{parent},
  m_recvData(false), m_quit(false), m_buffer(nullptr), m_bufferSize(0), m_dataSize(0){

  connect(this, &DeviceSocket::readyRead, this, &DeviceSocket::onReadyRead);
  connect(this, &DeviceSocket::disconnected, this, &DeviceSocket::quitNotify);
  connect(this, &DeviceSocket::aboutToClose, this, &DeviceSocket::quitNotify);
}

qint32 DeviceSocket::subThreadRecvData(quint8* buffer, quint32 bufferSize){

  //保证该函数在子线程中调用
  Q_ASSERT(QCoreApplication::instance()->thread() != QThread::currentThread());

  if( m_quit != 0 ) return 0;

  QMutexLocker locker(&m_mutex);

  m_buffer = buffer;
  m_bufferSize = bufferSize;
  m_dataSize = 0;

  while( !m_recvData ){

    m_recvDataCond.wait(&m_mutex);
  }

  m_recvData = false;

  return m_dataSize; //返回实际收到的数据
}

void DeviceSocket::onReadyRead(){

  QMutexLocker locker(&m_mutex);
  if( m_buffer != nullptr && bytesAvailable() > 0 ){

    qint64 readSize = qMin(bytesAvailable(), (qint64)m_bufferSize);
    m_dataSize = read((char*)m_buffer, readSize);

    m_buffer = nullptr;
    m_bufferSize = 0;
    m_recvData = true;
    m_recvDataCond.wakeOne();
  }
}

void DeviceSocket::quitNotify(){

  m_quit = true;
  QMutexLocker locker(&m_mutex);
  m_buffer = nullptr;
  m_bufferSize = 0;
  m_recvData = true;
  m_dataSize = 0;
  m_recvDataCond.wakeOne();
}

DeviceSocket::~DeviceSocket(){

}
