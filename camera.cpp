#include "camera.h"

Camera::Camera()
{

    trackingMouse = false;
    lastTime = QTime::currentTime();
    eye = QVector3D(0.0, 1.0, 2.0);
    at = QVector3D(0.0, 0.0, 0.0);
    up = QVector3D(0.0, 1.0, 0.0);

    vp=QVector3D(0.0,0.0,0.0);
    doubleRotationCameraX=0;
    doubleRotationCameraY=0;
    direction= QVector3D(0,0,0.2);
    firstPassInMouseMove=false;
}
void Camera::toFront(double scale){
    double atY=at.y();
    double eyeY=eye.y();
    QVector3D direction=this->direction*scale;
    at=(at-direction);
    eye=(eye-direction);
    eye.setY(eyeY);
    at.setY(atY);

}
void Camera::toLeft(double scale){
    //eye.setY(0.0);
    double atY=at.y();
    double eyeY=eye.y();
    QVector3D direction=rotationXZByY(this->direction,1.57)*scale;

    at=(at-direction);
    eye=(eye-direction);
    //eye.setY(1.0);
    eye.setY(eyeY);
    at.setY(atY);

}
void Camera::toRight(double scale){
    //eye.setY(0.0);
    double atY=at.y();
    double eyeY=eye.y();
    QVector3D direction=rotationXZByY(this->direction,-1.57)*scale;

    at=(at-direction);
    eye=(eye-direction);
    //eye.setY(1.0);
    eye.setY(eyeY);
    at.setY(atY);
}
void Camera::toBack(double scale){
    //eye.setY(0.0);
    double atY=at.y();
    double eyeY=eye.y();
    QVector3D direction=this->direction*scale;

    at=(at+direction);
    eye=(eye+direction);
    //eye.setY(1.0);
    eye.setY(eyeY);
    at.setY(atY);
}

void Camera::rotateAxisX(double scale){
    QVector3D atMinusEyeXPlane=(at-eye);
    double y=at.y();
    atMinusEyeXPlane.setY(0);
    doubleRotationCameraX=doubleRotationCameraX+(-scale);
    atMinusEyeXPlane =  rotationXZByY(atMinusEyeXPlane,3*(-scale));

    at=eye+atMinusEyeXPlane;

    direction=rotationXZByY(QVector3D(0,0,0.2),3*doubleRotationCameraX);

    at.setY(y);

}
void Camera::rotateAxisY(double scale){
    if(at.y()<-1.0 &&at.y()> 1.0) return;
     at.setY(at.y()+scale);
}
void Camera::mouseMove(const QPointF &p)
{
    if(firstPassInMouseMove){
        lastPos3D=mousePosTo3D(p);
        firstPassInMouseMove=false;
        return;
    } 


    QTime currentTime = QTime::currentTime();
    vp = mousePosTo3D(p);

    if(sqrt((lastPos3D.x()-vp.x())*(lastPos3D.x()-vp.x()))>0.06 ){
        lastPos3D=vp;
        return;
    }
    QVector3D atMinusEyeXPlane=(at-eye);
    atMinusEyeXPlane.setY(0);
    doubleRotationCameraX=doubleRotationCameraX+(lastPos3D.x()-vp.x());
    doubleRotationCameraY=doubleRotationCameraY+(lastPos3D.y()-vp.y());
    atMinusEyeXPlane =  rotationXZByY(atMinusEyeXPlane,3*(lastPos3D.x()-vp.x()));

    at=eye+atMinusEyeXPlane;
    at.setY(vp.y());
    direction=rotationXZByY(QVector3D(0,0,0.2),3*doubleRotationCameraX);
    lastPos3D=vp;
    lastTime = currentTime;

}

void Camera::mousePress(const QPointF &p)
{

    lastTime = QTime::currentTime();

    //lastPos3D = mousePosTo3D(p);
    trackingMouse = true;
}

void Camera::mouseRelease(const QPointF &p)
{



    trackingMouse = false;
}

void Camera::resizeViewport(int width, int height)
{

    viewportWidth = static_cast<double>(width);
    viewportHeight = static_cast<double>(height);
}

double Camera::getRotation()
{

 return -180*(vp.x());

}
QVector3D Camera::getDirection(){
    return direction;
}

QVector3D Camera::mousePosTo3D(const QPointF &p)
{

    return QVector3D(2.0 * p.x() / viewportWidth - 1.0,
                     1.0 - 2.0 * p.y() / viewportHeight,
                     0.0);
}


QVector3D Camera::rotationXZByY(QVector3D vet,double angle){
   QVector3D ret;
   double mat[3][3];
   mat[0][0]=0;mat[1][0]=0;mat[2][0]=0;
   mat[0][1]=0;mat[1][1]=0;mat[2][1]=0;
   mat[0][2]=0;mat[1][2]=0;mat[2][2]=0;
   mat[0][0]=cos(angle);
   mat[0][2]=sin(angle);
   mat[2][0]=-sin(angle);
   mat[2][2]=cos(angle);
   mat[1][1]=1;
   ret.setX(mat[0][0]*vet.x()+mat[0][1]*vet.y()+mat[0][2]*vet.z());
   ret.setY(mat[1][0]*vet.x()+mat[1][1]*vet.y()+mat[1][2]*vet.z());
   ret.setZ(mat[2][0]*vet.x()+mat[2][1]*vet.y()+mat[2][2]*vet.z());
   return ret;
}
QVector3D Camera::rotationXYByZ(QVector3D vet,double angle){
   QVector3D ret;
   double mat[3][3];
   mat[0][0]=0;mat[1][0]=0;mat[2][0]=0;
   mat[0][1]=0;mat[1][1]=0;mat[2][1]=0;
   mat[0][2]=0;mat[1][2]=0;mat[2][2]=0;
   mat[0][0]=cos(angle);
   mat[0][1]=-sin(angle);
   mat[1][0]=sin(angle);
   mat[1][1]=cos(angle);
   mat[2][2]=1;
   ret.setX(mat[0][0]*vet.x()+mat[0][1]*vet.y()+mat[0][2]*vet.z());
   ret.setY(mat[1][0]*vet.x()+mat[1][1]*vet.y()+mat[1][2]*vet.z());
   ret.setZ(mat[2][0]*vet.x()+mat[2][1]*vet.y()+mat[2][2]*vet.z());
   return ret;
}
QVector3D Camera::rotationYZByX(QVector3D vet,double angle){
   QVector3D ret;
   double mat[3][3];
   mat[0][0]=0;mat[1][0]=0;mat[2][0]=0;
   mat[0][1]=0;mat[1][1]=0;mat[2][1]=0;
   mat[0][2]=0;mat[1][2]=0;mat[2][2]=0;
   mat[0][0]=1;
   mat[1][1]=cos(angle);
   mat[1][2]=-sin(angle);
   mat[2][1]=sin(angle);
   mat[2][2]=cos(angle);

   ret.setX(mat[0][0]*vet.x()+mat[0][1]*vet.y()+mat[0][2]*vet.z());
   ret.setY(mat[1][0]*vet.x()+mat[1][1]*vet.y()+mat[1][2]*vet.z());
   ret.setZ(mat[2][0]*vet.x()+mat[2][1]*vet.y()+mat[2][2]*vet.z());
   return ret;
}
