#include <QBuffer>
#include <QIODevice>
#include "ControlEvent.h"

ControlEvent::ControlEvent(ControlEventType type){

  m_data.type = type;
}

void ControlEvent::write32(QBuffer& buffer, qint32 value){

  buffer.putChar(value >> 24);
  buffer.putChar(value >> 16);
  buffer.putChar(value >> 8);
  buffer.putChar(value);
}

void ControlEvent::write16(QBuffer& buffer, qint32 value){

  buffer.putChar(value >> 8);
  buffer.putChar(value);
}

void ControlEvent::writePosition(QBuffer& buffer, QRect& value){

  write16(buffer, value.left());
  write16(buffer, value.top());
  write16(buffer, value.width());
  write16(buffer, value.height());
}

QByteArray ControlEvent::serializeData(){

  QByteArray byteArray;
  QBuffer buffer(&byteArray);

  if( buffer.open(QIODevice::WriteOnly | QIODevice::Text) ){

    //构造指令
    //command type
    buffer.putChar(m_data.type);
    switch(m_data.type){
      case CET_MOUSE:
        buffer.putChar(m_data.MouseEvent.action);
        write32(buffer, m_data.MouseEvent.buttons);
        writePosition(buffer, m_data.MouseEvent.position);
        break;
      case CET_KEYCODE:
        break;
      case CET_TEXT:
        break;
      case CET_SCROLL:
        break;
      case CET_COMMAND:
        break;
      case CET_TOUCH:
        break;
    }

    buffer.close();
  }

  return byteArray;
}

void ControlEvent::setMouseEventData(AndroidMotioneventAction action, AndroidMotioneventButtons buttons, QRect rect){

  m_data.MouseEvent.action = action;
  m_data.MouseEvent.buttons = buttons;
  m_data.MouseEvent.position = rect;
}
