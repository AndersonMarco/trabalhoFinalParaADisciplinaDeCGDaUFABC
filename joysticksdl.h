#ifndef JOYSTICKSDL_H
#define JOYSTICKSDL_H
#include <QThread>
#include <QDebug>
#include<QObject>
class JoystickSDL : public QThread
{
     Q_OBJECT
public:
    JoystickSDL();

    void run();
    int axis0State=0;
    int axis1State=0;
    int axis2State=0;
    int axis3State=0;
public slots:
    void slotSendStatesOfAxisToOpenGlWidget();
signals:
    void signalMoveToFront(double scale);
    void signalMoveToBack(double scale);
    void signalMoveToLeft(double scale);
    void signalMoveToRight(double scale);
    void signalRotateAxis0(double scale);
    void signalRotateAxis3(double scale);
    void signalButtomPress(bool booleanUP, int buttom);

};

#endif // JOYSTICKSDL_H
