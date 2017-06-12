
#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <QVector3D>
#include <QQuaternion>
#include <QTime>
#include <QDebug>
#include <QMatrix4x4>
#include <cmath>

class Camera
{
public:
    QVector3D eye;
    QVector3D at;
    QVector3D up;
    Camera();
    void mouseMove(const QPointF& p);
    void mousePress(const QPointF& p);
    void mouseRelease(const QPointF& p);
    void toFront(double scale);
    void toLeft(double scale);
    void toRight(double scale);
    void toBack(double scale);
    void rotateAxisX(double scale);
    void rotateAxisY(double scale);
    //void KeyPress(int key);
    void resizeViewport(int width, int height);
    QVector3D getDirection();
    double getRotation();


protected:
    QVector3D rotationXZByY(QVector3D vet,double angle);
    QVector3D rotationXYByZ(QVector3D vet,double angle);
    QVector3D rotationYZByX(QVector3D vet,double angle);
    double doubleRotationCameraX;
    double doubleRotationCameraY;
    QVector3D lastPos3D;
    QVector3D direction;

    QVector3D atDirecton;
    QTime lastTime;
    bool trackingMouse;
    bool firstPassInMouseMove;

    double viewportWidth;
    double viewportHeight;
    QVector3D vp;
    QVector3D mousePosTo3D(const QPointF& p);
};

#endif
