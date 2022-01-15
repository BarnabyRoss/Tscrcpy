#ifndef __CONTROLEVENT_H__
#define __CONTROLEVENT_H__

#include <QEvent>
#include <QRect>
#include <QByteArray>
#include <QBuffer>
#include "input.h"
#include "keycodes.h"

class ControlEvent{

public:
  enum ControlEventType{

    CET_KEYCODE = 0,
    CET_TEXT,
    CET_MOUSE,
    CET_SCROLL,
    CET_COMMAND,
    CET_TOUCH
  };

protected:
  void write32(QBuffer& buffer, qint32 value);
  void write16(QBuffer& buffer, qint32 value);
  void writePosition(QBuffer& buffer, QRect& value);

public:
  ControlEvent(ControlEventType);

  QByteArray serializeData();
  void setMouseEventData(AndroidMotioneventAction, AndroidMotioneventButtons, QRect);

private:
  struct ControlEventData{

    ControlEventType type;

    union{

      struct{

        AndroidMotioneventAction action;
        AndroidMotioneventButtons buttons;
        QRect position;

      } MouseEvent;

    };

    ControlEventData();
    ~ControlEventData();
  };

  ControlEventData m_data;

};

#endif // __CONTROLEVENT_H__
