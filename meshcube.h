#ifndef MESHCUBE_H
#define MESHCUBE_H

#include "mesh.h"
#include <QDebug>
class MeshCube : public Mesh
{

public:

    MeshCube();
    void setPosition(QVector3D position);
    QVector3D getPosition();
    void calculatePosition();
    bool haveForceOrVelocity();

    QVector3D velocity;
    QVector3D force;
    QVector3D position;    
    QVector3D positionAnt;

};

#endif // MESHCUBE_H
