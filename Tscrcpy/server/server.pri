QT       += core gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

HEADERS += \
  $$PWD/DeviceSocket.h \
  $$PWD/MyTcpServer.h \
  $$PWD/Server.h

SOURCES += \
  $$PWD/DeviceSocket.cpp \
  $$PWD/MyTcpServer.cpp \
  $$PWD/Server.cpp


