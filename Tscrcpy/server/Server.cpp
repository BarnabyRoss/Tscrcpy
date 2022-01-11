#include <QFileInfo>
#include <QCoreApplication>
#include <QHostAddress>
#include <QSize>
#include <QDebug>
#include "Server.h"

#define DEVICE_SERVER_PATH "/data/local/tmp/scrcpy-server.jar"
#define SOCKET_NAME "scrcpy"
#define DEVICE_NAME_FIELD_LENGTH 64

Server::Server(QObject* parent) : QObject(parent),
  m_serial(""), m_localPort(0), m_maxSize(0), m_bitRate(0), m_deviceSocket(nullptr){

  this->m_serverStartStep = SSS_NULL;
  this->m_serverPath = "";
  this->m_serverCopiedToDevice = false;
  this->m_enableReverse = false;

  connect(&m_workProcess, &AdbProcess::adbProcessResult, this, &Server::onAdbProcessResult);
  connect(&m_serverProcess, &AdbProcess::adbProcessResult, this, &Server::onAdbProcessResult);
  connect(&m_serverSocket, &QTcpServer::newConnection, this, [this](){

    QString deviceName;
    QSize deviceSize;
    //device name, device size
    m_deviceSocket = m_serverSocket.nextPendingConnection();
    if( m_deviceSocket != nullptr && m_deviceSocket->isValid() ){

      disableTunnelReverse();
      removeServer();
      emit connectToResult(true, deviceName, deviceSize);

    }else{

      emit connectToResult(false, deviceName, deviceSize);
      stop();
    }
  });
}

bool Server::start(const QString& serial, quint16 localPort, quint16 maxSize, quint32 bitRate){

  this->m_serial = serial;
  this->m_localPort = localPort;
  this->m_maxSize = maxSize;
  this->m_bitRate = bitRate;

  //start push server
  m_serverStartStep = SSS_PUSH;

  return startServerByStep();
}

void Server::stop(){

  if( m_deviceSocket != nullptr ){

    m_deviceSocket->close();
  }

  m_serverProcess.kill();
  disableTunnelReverse();
  removeServer();

  m_serverSocket.close();
}

QString Server::getServerPath(){

  if( m_serverPath.isEmpty() ){

    m_serverPath = QString::fromLocal8Bit(qgetenv("TSCRCPY_SERVER_PATH"));
    QFileInfo fileInfo(m_serverPath);
    if( m_serverPath.isEmpty() || !fileInfo.isFile() ){

      m_serverPath = QCoreApplication::applicationDirPath() + "/scrcpy-server.jar";
    }
  }

  return m_serverPath;
}

void Server::onAdbProcessResult(AdbProcess::ADB_EXEC_RESULT processResult){

  if( sender() == &m_workProcess ){

    if( m_serverStartStep != SSS_NULL ){

      switch(m_serverStartStep){

        case SSS_PUSH:
          if( processResult == AdbProcess::AER_SUCCESS_EXEC ){

            m_serverCopiedToDevice = true;
            m_serverStartStep = SSS_ENABLE_REVERSE;
            startServerByStep();
          }else if( processResult != AdbProcess::AER_SUCCESS_START ){

            qCritical("adb push failed!");
            m_serverStartStep = SSS_NULL;
            emit serverStartResult(false);  //发送信号报告push状态
          }
          break;
        case SSS_ENABLE_REVERSE:
          if( processResult == AdbProcess::AER_SUCCESS_EXEC ){

            m_serverStartStep = SSSS_EXECUTE_SERVER;
            startServerByStep();

          }else if( processResult != AdbProcess::AER_SUCCESS_START ){

            qCritical("adb reverse enable failed!");
            m_serverStartStep = SSS_NULL;
            //remove server 需要把第一步推送到Android端的apk删除
            removeServer();
            emit serverStartResult(false);
          }
        default:
          break;
      }
    }
  }

  if( sender() == &m_serverProcess ){

    if( m_serverStartStep == SSSS_EXECUTE_SERVER ){

      if( processResult == AdbProcess::AER_SUCCESS_START ){

        m_serverStartStep = SSS_RUNNING;
        m_enableReverse = true;
        emit serverStartResult(true);
        startServerByStep();
      }else if( processResult == AdbProcess::AER_ERROR_EXEC ){

        qCritical("shell start server failed!");
        m_serverStartStep = SSS_NULL;
        //disable reverse
        disableTunnelReverse();
        //remove server
        removeServer();
        emit serverStartResult(false);

      }
    }
  }
}

bool Server::startServerByStep(){

  bool ret = false;
  if( m_serverStartStep != SSS_NULL ){

    switch(m_serverStartStep){

      case SSS_PUSH:
        ret = pushServer();
        break;
      case SSS_ENABLE_REVERSE:
        ret = enableTunnelReverse();
        break;
      case SSSS_EXECUTE_SERVER:
        //监听本地代理端口
        m_serverSocket.setMaxPendingConnections(1);
        if( !m_serverSocket.listen(QHostAddress::LocalHost, m_localPort) ){
          qCritical() << "Could not listen to port " << m_localPort;
          m_serverStartStep = SSS_NULL;
          disableTunnelReverse();
          removeServer();
          emit serverStartResult(false);
          return false;
        }
        ret = executeServer();
        break;
      default:
        break;
    }
  }

  return ret;
}

bool Server::pushServer(){

  m_workProcess.push(m_serial, getServerPath(), DEVICE_SERVER_PATH);

  return true;
}

bool Server::enableTunnelReverse(){

  m_workProcess.reverse(m_serial, SOCKET_NAME, m_localPort);

  return true;
}

bool Server::disableTunnelReverse(){

  if( m_enableReverse ){

    AdbProcess* adbProcess = new AdbProcess();
    if( adbProcess != nullptr ){

      m_enableReverse = false;
      connect(adbProcess, &AdbProcess::adbProcessResult, this, [this](AdbProcess::ADB_EXEC_RESULT processResult){

        if( processResult != AdbProcess::AER_SUCCESS_START ){

          sender()->deleteLater();
        }
      });
      adbProcess->removeReverse(m_serial, DEVICE_SERVER_PATH);

    }else{

      return false;
    }

  }
  return true;
}

bool Server::removeServer(){

  if( m_serverCopiedToDevice ){

    m_serverCopiedToDevice = false;
    AdbProcess* adbProcess = new AdbProcess();
    if( adbProcess != nullptr ){

      connect(adbProcess, &AdbProcess::adbProcessResult, this, [this](AdbProcess::ADB_EXEC_RESULT processResult){

        if( processResult != AdbProcess::AER_SUCCESS_START ){

          sender()->deleteLater();
        }
      });
      adbProcess->removeFile(m_serial, DEVICE_SERVER_PATH);

    }else{

      return false;
    }
  }

  return true;
}

bool Server::executeServer(){

//adb shell CLASSPATH=/data/local/tmp/scrcpy-server.jar app_process /com.genymobile.scrcpy.Server 1080 20000 false
  QStringList args;
  args << "shell" << QString("CLASSPATH=%1").arg(DEVICE_SERVER_PATH);
  args << "app_process" << "/" << "com.genymobile.scrcpy.Server";
  args << QString::number(m_maxSize) << QString::number(m_bitRate) << "false" << "";

  m_serverProcess.execute(m_serial, args);

  return true;
}

bool Server::readInfo(QString& deviceName, QSize size){

  // abk001-----------------------0x0438 0x02d0
  //               64b            2b w   2b h  //前64字节代表设备名称，后2个字节代表设备宽，最后两字节设备的高
  unsigned char buf[DEVICE_NAME_FIELD_LENGTH + 4];
  if (m_deviceSocket->bytesAvailable() <= (DEVICE_NAME_FIELD_LENGTH + 4)) {
      m_deviceSocket->waitForReadyRead(300);
  }

  qint64 len = m_deviceSocket->read((char*)buf, sizeof(buf));
  if (len < DEVICE_NAME_FIELD_LENGTH + 4) {
      qInfo("Could not retrieve device information");
      return false;
  }
  buf[DEVICE_NAME_FIELD_LENGTH - 1] = '\0';
  deviceName = (char*)buf;
  size.setWidth((buf[DEVICE_NAME_FIELD_LENGTH] << 8) | buf[DEVICE_NAME_FIELD_LENGTH + 1]);
  size.setHeight((buf[DEVICE_NAME_FIELD_LENGTH + 2] << 8) | buf[DEVICE_NAME_FIELD_LENGTH + 3]);

  return true;
}
