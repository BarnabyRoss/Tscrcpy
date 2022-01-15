#include <QOpenGLFunctions>
#include "QYUVOpenGLWidget.h"

QYUVOpenGLWidget::QYUVOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent){

}

void QYUVOpenGLWidget::initializeGL(){

  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
}

void QYUVOpenGLWidget::paintGL(){


}

void QYUVOpenGLWidget::resizeGL(int w, int h){


}
