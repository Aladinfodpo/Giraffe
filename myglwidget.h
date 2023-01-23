#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    MyGLWidget(QWidget *parent);
    void sliderChanged(int i);

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    QOpenGLContext* m_context;
    unsigned int m_shaderProgram;

};
