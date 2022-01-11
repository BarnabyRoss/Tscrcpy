#ifndef __DECODER_H__
#define __DECODER_H__

#include <QObject>

extern "C"{

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

}

class Decoder : public QObject{

  Q_OBJECT
public:
  explicit Decoder(QObject *parent = nullptr);

  static bool init();
};

#endif // __DECODER_H__
