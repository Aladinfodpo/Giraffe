#include "myglwidget.h"
#include <QOpenGLContext>
#include <QMessageBox>
#include <QImage>
#include <QDebug>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
     m_vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec2 texCoord;\n"
        "uniform mat4 matrix;\n"
        "out vec2 tex;\n"
        "void main() {\n"
        "   tex = texCoord;\n"
        "   gl_Position = matrix * vec4(position,1.0);\n"
        "}";

    m_fragmentShaderSource =
        "#version 460 core\n"
        "in vec2 tex;\n"
        "layout(location = 5) uniform sampler2D texture1;\n"
        "void main() {\n"
        "   gl_FragColor = texture(texture1, tex);\n"
        "}";

}

void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback( [](GLenum, GLenum type, GLuint, GLenum, GLsizei, const GLchar *message, const void *param){emit ((MyGLWidget*)param)->errorRaised(message, type == GL_DEBUG_TYPE_ERROR);}, this);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_TRUE);

    const float vertices[] = { //Coord
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f,
    };
    const int nbData = (3+2) * sizeof(float);

    // load and create a texture
    // -------------------------
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

    //bind buffers
    uint32_t vbuffer;
    glGenBuffers(1, &vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,false,nbData,nullptr);
    glVertexAttribPointer(1,2,GL_FLOAT,false,nbData,(void*)12);

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

    float matrix[] = {
         1.0f,  0.0f, 0.0f, 0.f,
         0.0f,  1.0f, 0.0f, 0.f,
         0.0f,  0.0f, 1.0f, 0.f,
         0.0f,  0.0f, 0.0f, 1.f,
    };

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "matrix"), 1, false, matrix);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "texture1"), 8);
}

void MyGLWidget::sliderChanged(int i)
{
    makeCurrent();
    const auto cosx = std::cos(i/100.f*2*3.14f);
    const auto sinx = std::sin(i/100.f*2*3.14f);
    float matrix[] = {
         cosx, -sinx, 0.0f, 0.f,
         sinx,  cosx, 0.0f, 0.f,
         0.0f,  0.0f, 1.0f, 0.f,
         0.0f,  0.0f, 0.0f, 1.f,
    };

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "matrix"), 1, false, matrix);
    paintGL();
}

void MyGLWidget::paintGL()
{
    glClearColor(0.5,0.,0.5,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
    sliderChanged(0);
}

void MyGLWidget::dumpWarnings(bool dump){
    makeCurrent();
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, dump ? GL_DEBUG_SEVERITY_HIGH : GL_DONT_CARE, 0, NULL, GL_TRUE);
}
