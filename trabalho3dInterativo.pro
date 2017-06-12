#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T16:29:45
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = trabalho3dInterativo
TEMPLATE = app


LIBS += -lSDL
SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    fileoperationsfromresource.cpp \
    mesh.cpp \
    light.cpp \
    material.cpp \
    camera.cpp \
    meshcube.cpp \
    meshfromfile.cpp \
    qthreadphysicsmeshcube.cpp \
    meshsphere.cpp \
    joysticksdl.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    fileoperationsfromresource.h \
    mesh.h \
    light.h \
    material.h \
    camera.h \
    meshcube.h \
    meshfromfile.h \
    qthreadphysicsmeshcube.h \
    meshsphere.h \
    joysticksdl.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    shader.frag \
    shader.vert \
    floor.vert \
    floor.frag \
    img/earth.jpg \
    img/venus.jpg \
    img/clouds.jpg \
    paintscene.frag \
    paintscene.vert \
    mesh.frag \
    mesh.vert
