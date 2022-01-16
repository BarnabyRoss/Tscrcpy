#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <QObject>
#include <QPointer>


class DeviceSocket;
class ControlEvent;

class Controller : public QObject{

  Q_OBJECT
public:
  explicit Controller(QObject* parent = nullptr);

  void setDeviceSocket(DeviceSocket* deviceSoket);
  bool sendControl(QByteArray& byteArray);
  void postControlEvent(ControlEvent* controlEvent);

protected:
  bool event(QEvent* evt);

private:
  QPointer<DeviceSocket> m_deviceSocket;

};

#endif // __CONTROLLER_H__
