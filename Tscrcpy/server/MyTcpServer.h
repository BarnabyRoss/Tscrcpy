#ifndef __MYTCPSERVER_H__
#define __MYTCPSERVER_H__

#include <QTcpServer>
#include <QObject>

class MyTcpServer : public QTcpServer{

  Q_OBJECT

public:
  explicit MyTcpServer(QObject* parent = nullptr);

  virtual void incomingConnection(qintptr handle);
};

#endif // __MYTCPSERVER_H__
