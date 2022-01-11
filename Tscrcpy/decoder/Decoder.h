#ifndef __DECODER_H__
#define __DECODER_H__

#include <QObject>

class Decoder : public QObject{

  Q_OBJECT
public:
  explicit Decoder(QObject *parent = nullptr);

signals:

};

#endif // __DECODER_H__
