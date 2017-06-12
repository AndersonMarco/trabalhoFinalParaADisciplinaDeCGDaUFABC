#include "cameranorotation.h"

CameraNoRotation::CameraNoRotation()
{

}

void CameraNoRotation::mouseMove(const QPointF &p)
{
    if(firstPassInMouseMove){
        lastPos3D=mousePosTo3D(p);
        firstPassInMouseMove=false;
        return;
    }


    QTime currentTime = QTime::currentTime();
    vp = mousePosTo3D(p);

    //at=eye+atMinusEyeXPlane;
    at.setY(vp.y());
    direction=rotationXZByY(QVector3D(0,0,0.2),3*doubleRotationCameraX);
    lastPos3D=vp;
    lastTime = currentTime;

}
