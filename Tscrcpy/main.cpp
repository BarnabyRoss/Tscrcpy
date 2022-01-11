#include "Widget.h"
#include "Decoder.h"
#include <QApplication>

int main(int argc, char *argv[]){

  QApplication a(argc, argv);
  Widget w;

  qputenv("TSCRCPY_ADB_PATH", "..\\Tscrcpy\\third_party\\adb\\winAdb\\adb.exe");
  qputenv("TSCRCPY_SERVER_PATH", "..\\Tscrcpy\\third_party\\scrcpy-server.jar");

  Decoder::init();

  w.show();

  return a.exec();
}
