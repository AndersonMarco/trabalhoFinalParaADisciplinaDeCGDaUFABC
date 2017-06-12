#ifndef CAMERANOROTATION_H
#define CAMERANOROTATION_H
#include "camera.h"

class CameraNoRotation: public Camera
{
public:
    CameraNoRotation();
    void mouseMove(const QPointF &p);

};

#endif // CAMERANOROTATION_H
