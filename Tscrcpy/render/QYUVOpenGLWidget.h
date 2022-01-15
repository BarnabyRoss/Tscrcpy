#ifndef __QYUVOPENGLWIDGET_H__
#define __QYUVOPENGLWIDGET_H__

#include <QOpenGLWidget>
#include <QWidget>

class QYUVOpenGLWidget : public QOpenGLWidget{

public:
  QYUVOpenGLWidget(QWidget* parent = nullptr);

protected:
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL(int w, int h);
};

#endif // __QYUVOPENGLWIDGET_H__
