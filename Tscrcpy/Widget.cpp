#include <QDebug>
#include "Widget.h"
#include "AdbProcess.h"

Widget::Widget(QWidget* parent) : QWidget(parent), m_startBtn(this), m_stopBtn(this){

  m_startBtn.resize(80, 55);
  m_startBtn.move(120, 85);
  m_startBtn.setText("start");

  m_stopBtn.resize(80, 55);
  m_stopBtn.move(210, 85);
  m_stopBtn.setText("stop");

  connect(&m_startBtn, SIGNAL(clicked()), this, SLOT(onStartBtnClicked()));
  connect(&m_stopBtn, SIGNAL(clicked()), this, SLOT(onStopBtnClicked()));

  connect(&m_server, &Server::serverStartResult, this, [this](bool success){
    qDebug() << "server start success : " << success;
  });
  connect(&m_server, &Server::connectToResult, this, [this](bool success, const QString& deviceName, const QSize& size){
    qDebug() << "connect to success : " << success << deviceName << size;
  });

  this->resize(400, 280);
}

void Widget::onStartBtnClicked(){

  m_server.start("", 27183, 720, 8000000);
  //QString program = "..\\Tscrcpy\\third_party\\adb\\winAdb\\adb.exe";
  //QStringList arguments;
  //AdbProcess* process = new AdbProcess(this);
  //process->push("", "..\\Tscrcpy\\third_party\\scrcpy-server.jar", "/data/local/tmp/scrcpy-server.jar");
//  process->push("", "C:\\Users\\Lenovo\\Desktop\\test.txt", "/sdcard/test.txt");

}

void Widget::onStopBtnClicked(){

  m_server.stop();
}

void testAdbProcess(){

  //QString program = "..\\Tscrcpy\\third_party\\adb\\winAdb\\adb.exe";
  //QStringList arguments;
  //arguments << "devices";
  //arguments << "shell" << "ip" << "-f" << "inet" << "addr" << "show" << "wlan0";

  //AdbProcess* process = new AdbProcess(this);
  //process->start(program, arguments);
  //process->start(AdbProcess::getAdbPath(), nullptr);
  //connect(process, &AdbProcess::adbProcessResult, this, [this, process](AdbProcess::ADB_EXEC_RESULT processResult){
    //qDebug() << ">>>>>>>>>>>" << processResult;
    //if( AdbProcess::AER_SUCCESS_EXEC == processResult ){
      //QStringList res = process->getDeviceSerialFromStdOut();
      //for(int i = 0; i < res.count(); ++i) qDebug() << res[i];

      //QString ip = process->getDeviceIpFromStdOut();
      //qDebug() << "ip = " << ip;
    //}
  //});
  //process->execute("", arguments);
  //process->push("", "C:\\Users\\Lenovo\\Desktop\\test.txt", "/sdcard/test.txt");
  //process->removeFile("", "/sdcard/test.txt");
  //process->reverse("", "Tscrcpy", 5037);
  //process->removeReverse("", "Tscrcpy");
}

void Widget::onAdbProcessResult(AdbProcess::ADB_EXEC_RESULT processResult){

  qDebug() << ">>>>>>>>>>>" << processResult;
}

Widget::~Widget(){

}

