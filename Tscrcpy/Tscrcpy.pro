QT       += core gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    Widget.cpp

HEADERS += \
    Widget.h

#添加子模块
include($$PWD/adb/adb.pri)
include($$PWD/server/server.pri)
include($$PWD/decoder/decoder.pri)
include($$PWD/common/common.pri)
include($$PWD/render/render.pri)
include($$PWD/android/android.pri)
include($$PWD/inputControl/inputControl.pri)

#添加目录
INCLUDEPATH +=                                \
    $$PWD/adb                                 \
    $$PWD/server                              \
    $$PWD/decoder                             \
    $$PWD/third_party/ffmpeg/include          \
    $$PWD/common                              \
    $$PWD/render                              \
    $$PWD/android                             \
    $$PWD/inputControl                        \

#添加依赖的外部库
LIBS +=                                        \
    -L$$PWD/third_party/ffmpeg/lib -lavformat  \
    -L$$PWD/third_party/ffmpeg/lib -lavcodec   \
    -L$$PWD/third_party/ffmpeg/lib -lavutil    \
    -L$$PWD/third_party/ffmpeg/lib -lswscale   \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
