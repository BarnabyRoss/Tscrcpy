#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <QWidget>

class Widget : public QWidget{

  Q_OBJECT

public:
  Widget(QWidget* parent = nullptr);
  ~Widget();
};
#endif // __WIDGET_H__
