QT += core gui opengl widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    myglwidget.cpp

HEADERS += \
    mainwindow.h \
    mesh.h \
    myglwidget.h

INCLUDEPATH += $$PWD/../Packages/include/glm

FORMS += mainwindow.ui

TARGET = Giraffe

target.path = /opt/$${TARGET}/bin
INSTALLS += target

LIBS += opengl32.lib

RESOURCES += data/ressource.qrc

RC_ICONS = data/giraffe.ico
