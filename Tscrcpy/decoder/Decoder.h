#ifndef __DECODER_H__
#define __DECODER_H__

#include <QObject>
#include <QThread>
#include <QPointer>
#include "DeviceSocket.h"

extern "C"{

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

}

class Decoder : public QThread{

  Q_OBJECT
public:
  explicit Decoder(QObject *parent = nullptr);

  static bool init();
  static void deInit();

  void setDeviceSocket(DeviceSocket* deviceSocket);
  qint32 recvData(uint8_t* buf, int buf_size);

protected:
  void run() override;
  void pushFrame();

signals:
  void onDecodeStop();

private:
  Frames m_frames;  //用于保存解码帧

  QPointer<DeviceSocket> m_deviceSocket;
  bool m_quit;
};

#endif // __DECODER_H__
