
#include <QCoreApplication>
#include "Controller.h"
#include "DeviceSocket.h"
#include "ControlEvent.h"

Controller::Controller(QObject* parent) : QObject{parent}{

}

void Controller::setDeviceSocket(DeviceSocket* deviceSoket){

  this->m_deviceSocket = deviceSoket;
}

bool Controller::sendControl(QByteArray& byteArray){

  if( byteArray.isEmpty() ) return false;

  qint32 len = 0;
  if( m_deviceSocket != nullptr ){

    len = m_deviceSocket->write(byteArray.data(), byteArray.length());
  }

  return len == byteArray.length();
}

void Controller::postControlEvent(ControlEvent* controlEvent){

  QCoreApplication::postEvent(this, controlEvent);
}

bool Controller::event(QEvent* evt){

  if( evt != nullptr && evt->type() == ControlEvent::Control ){

    ControlEvent* controlEvent = dynamic_cast<ControlEvent*>(evt);
    if( controlEvent != nullptr ){

      sendControl(controlEvent->serializeData());
    }
    return true;
  }else{

    return QObject::event(evt);
  }
}
