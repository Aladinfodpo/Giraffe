#include "myglwidget.h"
#include <QOpenGLContext>
#include <QMessageBox>
#include <QImage>
#include <QWheelEvent>
#include <QDebug>
#include <QMimeData>
#include <chrono>

#include "gtx/string_cast.hpp"

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent),
camPos(glm::vec3(1.0f,0,1.f)), lookAt(glm::vec3(0,0,0.f)), fov(80.f){
     m_vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec2 texCoord;\n"
        "layout(location = 2) in vec3 normal;\n"
        "uniform mat4 matrix;\n"
        "uniform vec3 cam;\n"
        "out vec2 tex;\n"
        "out vec3 nor;\n"
        "out vec3 view;\n"
        "void main() {\n"
        "   tex = texCoord;\n"
        "   nor = normal;\n"
        "   view = cam-position;\n"
        "   gl_Position = matrix * vec4(position,1.0);\n"
        "}";

    m_fragmentShaderSource =
        "#version 460 core\n"
        "in vec2 tex;\n"
        "in vec3 nor;\n"
        "in vec3 view;\n"
        "layout(location = 3) uniform float in_x;\n"
        "layout(location = 5) uniform sampler2D texture1;\n"
        "void main() {\n"
        "vec4 spec = vec4(0.0);\n"
        "vec4 diff = texture(texture1, tex);\n"
        "vec3 n = normalize(nor);\n"
        "vec3 l = normalize(vec3(5.,5.,5.));\n"
        "vec3 e = normalize(view);\n"
        "float intensity = max(dot(n,l), 0.0);\n"
        "\n"
        "if (intensity > 0.0) { \n"
        "   vec3 h = normalize(l + e);\n"
        "    float intSpec = max(dot(h,n), 0.0);\n"
        "    spec = diff * pow(intSpec, 60.);\n"
        "}\n"
        "   gl_FragColor = intensity * diff + spec;\n"
        "};";

    connect(this, &MyGLWidget::needsRedraw, this, &MyGLWidget::redraw);
}

void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback( [](GLenum, GLenum type, GLuint, GLenum, GLsizei, const GLchar *message, const void *param){emit ((MyGLWidget*)param)->errorRaised(message, type == GL_DEBUG_TYPE_ERROR);}, this);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_TRUE);

    glEnable(GL_DEPTH_TEST);

    m_mesh.addDefaultPlane();

    // load and create a texture
    QImage image;
    image.load(":/checker.png");

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    //Hack car qt unbind la dernière texture
    glActiveTexture(GL_TEXTURE0);

    //create vao
    uint32_t vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);//pos
    glEnableVertexAttribArray(1);//TexCoord
    glEnableVertexAttribArray(2);//Normal

    //bind buffers
    glGenBuffers(1, &m_vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbuffer);
    glBufferData(GL_ARRAY_BUFFER, m_mesh.getSizeData(), m_mesh.getData(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, Point3D::sizePoint, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, Point3D::sizePoint, (void*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, Point3D::sizePoint, (void*)20);

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLchar const* files[] = { m_vertexShaderSource.c_str() };
    GLint lengths[]       = { (int)m_vertexShaderSource.size()  };
    glShaderSource(vShader, 1, files, lengths);
    glCompileShader(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar const* filesF[] = { m_fragmentShaderSource.c_str() };
    GLint lengthsF[]       = { (int)m_fragmentShaderSource.size()  };
    glShaderSource(fShader, 1, filesF, lengthsF);
    glCompileShader(fShader);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vShader);
    glAttachShader(m_shaderProgram, fShader);
    glLinkProgram(m_shaderProgram);
    glUseProgram(m_shaderProgram);
    glDetachShader(m_shaderProgram, fShader);

    glUniform1i(glGetUniformLocation(m_shaderProgram, "texture1"), 8);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "in_x"), 0.5f);

    rotate(0.f, 0.f, 0.f);
}

void MyGLWidget::paintGL()
{
    static auto prevTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> dur = std::chrono::steady_clock::now() - prevTime;

    static int nbFrame = 0;
    nbFrame++;
    if(dur.count() > .3){
        emit fpsCalculated(QString::number(nbFrame/dur.count(),'f',0) + " fps");
        prevTime = std::chrono::steady_clock::now();
        nbFrame = 0;
    }

    glClearColor(0.5,0.,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, m_mesh.getNbPoint());
    update();
}

void MyGLWidget::compileFrag(QString frag){
    m_fragmentShaderSource = frag.toStdString();

    makeCurrent();

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar const* filesF[] = { m_fragmentShaderSource.c_str() };
    GLint lengthsF[]       = { (int)m_fragmentShaderSource.size()  };
    glShaderSource(fShader, 1, filesF, lengthsF);
    glCompileShader(fShader);

    glAttachShader(m_shaderProgram, fShader);
    glLinkProgram(m_shaderProgram);
    glUseProgram(m_shaderProgram);
    glDetachShader(m_shaderProgram, fShader);

    glUniform1i(glGetUniformLocation(m_shaderProgram, "texture1"), 8);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "in_x"), 0.5f);
    matrixChanged();
}

void MyGLWidget::dumpWarnings(bool dump){
    makeCurrent();
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, dump ? GL_DEBUG_SEVERITY_HIGH : GL_DONT_CARE, 0, NULL, GL_TRUE);
}

void MyGLWidget::updateInX(float in_x){
    makeCurrent();
    glUniform1f(glGetUniformLocation(m_shaderProgram, "in_x"), in_x);
    emit needsRedraw();
}

void MyGLWidget::wheelEvent(QWheelEvent *event){
    rotate(0.f,0.f,- event->angleDelta().y()/300.f);

    event->accept();
}

void MyGLWidget::redraw(){
    paintGL();
}

void MyGLWidget::takeScreenShot(){
    makeCurrent();
    const auto w  = width();
    const auto h = height();
    BYTE* pixels = new BYTE[3 * w * h];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    QImage image = QImage(w, h, QImage::Format_RGB888);
    for (int i = 0; i < h; ++i)
        memcpy(image.scanLine(i), pixels + 3 * (h-1-i)* w, w * 3);
    delete[] pixels;

    emit snpashotAvailable(image);
}

void MyGLWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasUrls())
        return;

    for(const QUrl &url :  event->mimeData()->urls()) {
            QString fileName = url.toLocalFile();
            if(fileName.endsWith(".obj"))
                event->acceptProposedAction();
        }
}

void MyGLWidget::meshChanged(){
    makeCurrent();
    glDeleteBuffers(1, &m_vbuffer);
    glGenBuffers(1, &m_vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbuffer);
    glBufferData(GL_ARRAY_BUFFER, m_mesh.getSizeData(), m_mesh.getData(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, Point3D::sizePoint, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, Point3D::sizePoint, (void*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, Point3D::sizePoint, (void*)20);
}

void MyGLWidget::dropEvent(QDropEvent *event)
{
    for(const QUrl &url : event->mimeData()->urls()) {
            QString fileName = url.toLocalFile();
            if(fileName.endsWith(".obj")){
                event->acceptProposedAction();
                loadObj(fileName);
                return;
            }
        }
}

void MyGLWidget::matrixChanged(){
    makeCurrent();
    glm::mat4 camMatrix = glm::lookAt(
        camPos,            // eye
        lookAt,            // lookat
        glm::vec3(0,1,0)   // up
    );

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(fov),               // The vertical Field of View, in radians
        width() / float(height()),       // Aspect Ratio
        0.1f,                            // Near clipping plane
        100.0f                           // Far clipping plane
    );

    //emit errorRaised(("Camera Matrix : " + glm::to_string(camMatrix)).c_str(), false);
    //emit errorRaised(("Projection Matrix : " + glm::to_string(projectionMatrix)).c_str(), false);

    camMatrix = projectionMatrix * camMatrix;
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "matrix"), 1, false, &camMatrix[0][0]);
    glUniform3f(glGetUniformLocation(m_shaderProgram, "cam"), camPos[0], camPos[1], camPos[2]);
    emit needsRedraw();
}

void MyGLWidget::resizeGL(int, int){
    matrixChanged();
}

void MyGLWidget::rotate(float dT, float dP, float dR){
    static float theta = 3.1415f/2.f;
    static float phi = 3.1415f/2.f;
    static float r = 2.f;

    float f;
    theta = 2*3.1415*std::modf((theta +dT)/(2*3.1415), &f);
    phi = std::clamp(phi+dP, 0.0001f, 3.1414f);
    r = std::clamp(r+dR, 0.001f, 100.f);

    //emit errorRaised("Phi : " + QString::number(phi) +" + " + QString::number(dP) + ", Theta : " + ", Theta : " + QString::number(theta) + " + " + QString::number(dT), false);

    camPos = glm::vec3(r*std::sin(phi)*std::cos(theta) , r*std::cos(phi), r*std::sin(phi)*std::sin(theta));
    matrixChanged();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){
        rotate((event->x()-m_lastPosMouseX)/300.f, -(event->y()-m_lastPosMouseY)/300.f);
        m_lastPosMouseX = event->x();
        m_lastPosMouseY = event->y();
    }
}

void MyGLWidget::mousePressEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){
        m_lastPosMouseX = event->x();
        m_lastPosMouseY = event->y();
    }
}

void MyGLWidget::loadPlane(){
    m_mesh.clear();
    m_mesh.addDefaultPlane();
    meshChanged();
}

void MyGLWidget::loadSphere(){
    loadObj(":/sphere.obj");
}

void MyGLWidget::loadObj(QString filename){
    m_mesh.clear();
    m_mesh.loadObj(filename);
    meshChanged();
    emit needsRedraw();
}

