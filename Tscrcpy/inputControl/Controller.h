#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <QObject>

class Controller : public QObject{

  Q_OBJECT
public:
  explicit Controller(QObject* parent = nullptr);

signals:

};

#endif // __CONTROLLER_H__
