#include "Decoder.h"

Decoder::Decoder(QObject *parent) : QObject{parent}{

}

bool Decoder::init(){

  if( avformat_network_init() ){  //返回1：初始化失败; 返回0: 初始化成功

    return false;
  }

  return true;
}
