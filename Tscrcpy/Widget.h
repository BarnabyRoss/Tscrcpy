#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <QWidget>
#include <QPushButton>
#include "AdbProcess.h"
#include "Server.h"

class Widget : public QWidget{

  Q_OBJECT

public:
  Widget(QWidget* parent = nullptr);
  ~Widget();

protected slots:
  void onStartBtnClicked();
  void onStopBtnClicked();
  void onAdbProcessResult(AdbProcess::ADB_EXEC_RESULT processResult);

private:
  QPushButton m_startBtn;
  QPushButton m_stopBtn;
  Server m_server;
};
#endif // __WIDGET_H__
