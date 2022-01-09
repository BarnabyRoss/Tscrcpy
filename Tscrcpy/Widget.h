#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <QWidget>
#include <QPushButton>
#include "AdbProcess.h"

class Widget : public QWidget{

  Q_OBJECT

public:
  Widget(QWidget* parent = nullptr);
  ~Widget();

protected slots:
  void onButtonClicked();
  void onAdbProcessResult(AdbProcess::ADB_EXEC_RESULT processResult);

private:
  QPushButton m_button;
};
#endif // __WIDGET_H__
