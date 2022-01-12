#include <QDebug>
#include "Decoder.h"


#define BUFSIZE 0x10000

Decoder::Decoder(QObject *parent) : QThread{parent}, m_quit(false){

}

bool Decoder::init(){

  if( avformat_network_init() ){  //返回1：初始化失败; 返回0: 初始化成功

    return false;
  }

  return true;
}

void Decoder::deInit(){

  avformat_network_deinit();
}

void Decoder::setDeviceSocket(DeviceSocket* deviceSocket){

  this->m_deviceSocket = deviceSocket;
}

qint32 Decoder::recvData(uint8_t* buf, int buf_size){

  if( buf == nullptr ) return 0;
  if( m_deviceSocket != nullptr ){

    qint32 len = m_deviceSocket->subThreadRecvData(buf, buf_size);
    if( len == -1 ) return AVERROR(errno);
    if( len == 0 ) return AVERROR_EOF;

    return len;
  }

  return AVERROR_EOF;
}

static qint32 readPacket(void* opaque, uint8_t* buf, int buf_size){

  Decoder* decoder = (Decoder*)opaque;
  if( decoder != nullptr ){

    return decoder->recvData(buf, buf_size);
  }
  return 0;
}

void Decoder::run(){

  unsigned char* decoderBuffer = nullptr;
  AVIOContext* avioCtx;
  bool isFormatCtxOpen = false;
  bool isCodecCtxOpen = false;

  decoderBuffer = (unsigned char*)av_malloc(BUFSIZE);
  if( !decoderBuffer ){
    qCritical("Could not allocate decoderBuffer!");
    goto runQuit;
  }
  avioCtx = avio_alloc_context(decoderBuffer, BUFSIZ, 0, this, readPacket, nullptr, nullptr);
  if( !avioCtx ){
    qCritical("Could not allocate avio content!");
    av_free(decoderBuffer);
    goto runQuit;
  }

  AVFormatContext* formatCtx;
  formatCtx = avformat_alloc_context();
  if( !formatCtx ){
    qCritical("Could not allocate format content!");
    goto runQuit;
  }
  formatCtx->pb = avioCtx;

  //解封装
  if( avformat_open_input(&formatCtx, nullptr, nullptr, nullptr) < 0 ){
    qCritical("Could not open video stream..");
    goto runQuit;
  }
  isFormatCtxOpen = true;

  AVCodec* codec = nullptr;
  AVCodecContext* codecCtx = nullptr;
  codec = avcodec_find_decoder(AV_CODEC_ID_H264);
  if( !codec ){
    qCritical("H.264 decoder not fond!");
    goto runQuit;
  }
  codecCtx = avcodec_alloc_context3(codec);
  if( !codecCtx ){
    qCritical("Could not allocate decoder content");
    goto runQuit;
  }

  if( avcodec_open2(codecCtx, codec, nullptr) < 0 ){

    qCritical("Could not open H.264 codec!");
    goto runQuit;
  }
  isCodecCtxOpen = true;

  AVPacket packet;
  av_init_packet(&packet);
  packet.data = nullptr;
  packet.size = 0;

  while( !m_quit && !av_read_frame(formatCtx, &packet) ){

    AVFrame* decodingFrame = m_frames->decodingFrame();

    int ret;
    if( (ret = avcodec_send_packet(codecCtx, &packet)) < 0 ){
      qCritical("Could not send video packet : %d", ret);
      goto runQuit;
    }
    if( decodingFrame != nullptr ){
      ret = avcodec_receive_frame(codecCtx, decodingFrame);
    }
    if( !ret ){
      pushFrame();
    }else if( ret != AVERROR(EAGAIN) ){
      qCritical("Could not receive video frame : %d", ret);
      goto runQuit;
    }

    av_packet_unref(&packet);  //clean packet
    if( avioCtx->eof_reached ) break;
  }
  qDebug() << "End of frames!";

runQuit:
  if( avioCtx ){
    av_freep(&avioCtx);
  }
  if( formatCtx && isFormatCtxOpen ){
    avformat_close_input(&formatCtx);
  }
  if( formatCtx ){
    avformat_free_context(formatCtx);
  }
  if( codecCtx && isCodecCtxOpen ){
    avcodec_close(codecCtx);
  }
  if( codecCtx ){
    avcodec_free_context(&codecCtx);
  }
  emit onDecodeStop();
}

void Decoder::pushFrame(){


}
