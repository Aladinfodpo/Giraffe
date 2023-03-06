#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QObject>

#include "mesh.h"
#include "ext/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent);
    void compileFrag(QString frag);
    void updateInX(float in_x);

    void loadPlane();
    void loadSphere();
    void loadObj(QString filename);

    std::string getFragmentShader(){return m_fragmentShaderSource;}

signals:
    void errorRaised(QString desc, bool isError);

    void fpsCalculated(QString text);

    void needsRedraw();

    void snpashotAvailable(QImage& image);

public slots:
    void dumpWarnings(bool dump);

    void redraw();

    void takeScreenShot();

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void wheelEvent(QWheelEvent* event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void meshChanged();

    void matrixChanged();

    void rotate(float dT,  float dP, float dr = 0.f);

    QOpenGLContext* m_context;
    unsigned int m_shaderProgram;

    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;

    uint32_t m_vbuffer;

    Mesh m_mesh;

    glm::vec3 camPos;
    glm::vec3 lookAt;
    float fov;

    int m_lastPosMouseX = 0;
    int m_lastPosMouseY = 0;
};
