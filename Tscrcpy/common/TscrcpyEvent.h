#ifndef __TSCRCPYEVENT_H__
#define __TSCRCPYEVENT_H__

#include <QEvent>

class TScrcpyEvent : public QEvent{

public:
  enum Type{

    DeviceSocket = QEvent::User + 1,
  };

public:
  TScrcpyEvent(Type type) : QEvent(static_cast<QEvent::Type>(type)) {}
};

class DeviceSocketEvent : public TScrcpyEvent{

public:
  DeviceSocketEvent() : TScrcpyEvent(DeviceSocket) {}
};

#endif // __TSCRCPYEVENT_H__
