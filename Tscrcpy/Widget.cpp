#include <QDebug>
#include "Widget.h"
#include "AdbProcess.h"

Widget::Widget(QWidget* parent) : QWidget(parent), m_button(this){

  m_button.resize(80, 55);
  m_button.move(120, 85);
  m_button.setText("clicked!");

  connect(&m_button, SIGNAL(clicked()), this, SLOT(onButtonClicked()));

  this->resize(400, 280);
}

void Widget::onButtonClicked(){

  //QString program = "..\\Tscrcpy\\third_party\\adb\\winAdb\\adb.exe";
  QStringList arguments;
  arguments << "devices";

  AdbProcess* process = new AdbProcess(this);
  //process->start(program, arguments);
  //process->start(AdbProcess::getAdbPath(), nullptr);
  connect(process, &AdbProcess::adbProcessResult, this, [this, process](AdbProcess::ADB_EXEC_RESULT processResult){
    qDebug() << ">>>>>>>>>>>" << processResult;
    if( AdbProcess::AER_SUCCESS_EXEC == processResult ){
      QStringList res = process->getDeviceSerialFromStdOut();
      for(int i = 0; i < res.count(); ++i) qDebug() << res[i];
    }
  });
  process->execute("", arguments);
  //process->push("", "C:\\Users\\Lenovo\\Desktop\\test.txt", "/sdcard/test.txt");
  //process->removeFile("", "/sdcard/test.txt");
  //process->reverse("", "scrcpy", 5037);
  //process->removeReverse("", "scrcpy");
}

void Widget::onAdbProcessResult(AdbProcess::ADB_EXEC_RESULT processResult){

  qDebug() << ">>>>>>>>>>>" << processResult;
}

Widget::~Widget(){

}

