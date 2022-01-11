#include "MyTcpServer.h"
#include "DeviceSocket.h"

MyTcpServer::MyTcpServer(QObject* parent) : QTcpServer{parent}{

}

void MyTcpServer::incomingConnection(qintptr handle){

  DeviceSocket* socket = new DeviceSocket();
  socket->setSocketDescriptor(handle);
  this->addPendingConnection(socket);
}
