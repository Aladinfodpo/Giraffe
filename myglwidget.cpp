#include "myglwidget.h"
#include <QOpenGLContext>
#include <QDebug>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) { }
void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    static const std::string vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec3 colAttr;\n"
        "uniform mat4 matrix;\n"
        "out vec3 col;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * vec4(position,1.0);\n"
        "}";

    static const std::string fragmentShaderSource =
        "#version 330 core\n"
        "in vec3 col;\n"
        "void main() {\n"
        "   gl_FragColor = vec4(col,1.0);\n"
        "}";

    float vertices[] = { //plus color
        -0.5f, -0.5f, 0.0f, 1.f,0.f,0.f,
         0.5f, -0.5f, 0.0f, 0.f,1.f,0.f,
         0.0f,  0.5f, 0.0f, 0.f,0.f,1.f,
    };

    //create vao
    uint32_t vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);//pos
    glEnableVertexAttribArray(1);//couleur

    //bind buffers
    uint32_t vbuffer;
    glGenBuffers(1, &vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,false,3*4*2,nullptr);
    glVertexAttribPointer(1,3,GL_FLOAT,false,3*4*2,(void*)12);

    auto vShader = glCreateShader(GL_VERTEX_SHADER);
    GLchar const* files[] = { vertexShaderSource.c_str() };
    GLint lengths[]       = { (int)vertexShaderSource.size()  };
    glShaderSource(vShader, 1,files, lengths);
    glCompileShader(vShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
    }

    auto fShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar const* filesF[] = { fragmentShaderSource.c_str() };
    GLint lengthsF[]       = { (int)fragmentShaderSource.size()  };
    glShaderSource(fShader, 1,filesF, lengthsF);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
    }

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vShader);
    glAttachShader(m_shaderProgram, fShader);
    glLinkProgram(m_shaderProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    float matrix[] = {
         1.0f,  0.0f, 0.0f, 0.f,
         0.0f,  1.0f, 0.0f, 0.f,
         0.0f,  0.0f, 1.0f, 0.f,
         0.0f,  0.0f, 0.0f, 1.f,
    };

    int matrixId = glGetUniformLocation(m_shaderProgram, "matrix");
    assert(matrixId != -1);

    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(matrixId,1,false,matrix);

}

void MyGLWidget::resizeGL(int w, int h)
{

}

void MyGLWidget::sliderChanged(int i)
{
    makeCurrent();
    auto cosx = std::cos(i/100.f*2*3.14f);
    auto sinx = std::sin(i/100.f*2*3.14f);
    float matrix[] = {
         cosx, -sinx, 0.0f, 0.f,
         sinx,  cosx, 0.0f, 0.f,
         0.0f,  0.0f, 1.0f, 0.f,
         0.0f,  0.0f, 0.0f, 1.f,
    };

    int matrixId = glGetUniformLocation(m_shaderProgram, "matrix");
    assert(matrixId != -1);

    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(matrixId,1,false,matrix);
    paintGL();
}

void MyGLWidget::paintGL()
{
    glClearColor(0.5,0.,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

