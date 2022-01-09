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

  QString program = "notepad";
  QStringList arguments;
  arguments << "C:\\Users\\Lenovo\\Desktop";

  AdbProcess* process = new AdbProcess(this);
  process->start(program, arguments);
}

Widget::~Widget(){

}

