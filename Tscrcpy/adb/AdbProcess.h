#ifndef __ADBPROCESS_H__
#define __ADBPROCESS_H__

#include <QProcess>

class AdbProcess : public QProcess{

public:
  AdbProcess(QObject* parent = nullptr);
};

#endif // __ADBPROCESS_H__
