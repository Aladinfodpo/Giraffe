#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QObject>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent);
    void sliderChanged(int i);
    void compileFrag(QString frag);

    std::string getFragmentShader(){return m_fragmentShaderSource;}

signals:
    void errorRaised(QString desc, bool isError);

public slots:
    void dumpWarnings(bool dump);

protected:
    void initializeGL() override;

    void resizeGL(int, int) override {}

    void paintGL() override;

    QOpenGLContext* m_context;
    unsigned int m_shaderProgram;

    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;
};
