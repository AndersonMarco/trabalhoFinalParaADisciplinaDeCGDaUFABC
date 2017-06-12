#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"joysticksdl.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTimer *timer= new QTimer(this);
    QTimer *timerJoystick= new QTimer(this);
    JoystickSDL *joystick=new JoystickSDL();
    joystick->start();
    ui->setupUi(this);
    ui->openGLWidget->setMouseTracking(true);
    QSurfaceFormat format;
    format.setSamples(16);
    ui->openGLWidget->setFormat(format);

    QObject::connect(this,SIGNAL(signalKeyPress(int)),ui->openGLWidget,SLOT(slotKeyPress(int)));
    QObject::connect(timer,SIGNAL(timeout()),ui->openGLWidget,SLOT(slotUpdateClouds()));
    QObject::connect(timerJoystick,SIGNAL(timeout()),joystick,SLOT(slotSendStatesOfAxisToOpenGlWidget()));
    QObject::connect(joystick,SIGNAL(signalMoveToFront(double)),ui->openGLWidget,SLOT(slotMoveToFront(double)));
    QObject::connect(joystick,SIGNAL(signalMoveToBack(double)),ui->openGLWidget,SLOT(slotMoveToBack(double)));
    QObject::connect(joystick,SIGNAL(signalMoveToLeft(double)),ui->openGLWidget,SLOT(slotMoveToLeft(double)));
    QObject::connect(joystick,SIGNAL(signalMoveToRight(double)),ui->openGLWidget,SLOT(slotMoveToRight(double)));
    QObject::connect(joystick,SIGNAL(signalRotateAxis0(double)),ui->openGLWidget,SLOT(slotRotateAxisXInCamera(double)));
    QObject::connect(joystick,SIGNAL(signalRotateAxis3(double)),ui->openGLWidget,SLOT(slotRotateAxisYInCamera(double)));
    QObject::connect(joystick,SIGNAL(signalButtomPress(bool,int)),ui->openGLWidget,SLOT(slotJoystickButtonPress(bool,int)));

    timer->start(90);
    timerJoystick->start(90);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent *event){
    emit signalKeyPress(event->key());

}
