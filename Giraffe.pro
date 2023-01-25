QT       += core gui opengl widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    myglwidget.cpp

HEADERS += \
    mainwindow.h \
    myglwidget.h

FORMS += mainwindow.ui

target.path = /opt/$${TARGET}/bin
INSTALLS += target

LIBS += opengl32.lib

RESOURCES +=  data/ressource.qrc

RC_ICON = data/giraffe.ico
