#ifndef MESHSPHERE_H
#define MESHSPHERE_H
#include<QVector3D>
#include "mesh.h"
#include "fileoperationsfromresource.h"

class MeshSphere : public Mesh
{
public:
    MeshSphere();
    void setPosition(QVector3D position);
    QVector3D getPosition();
    void calculatePosition();
    bool haveForceOrVelocity();

    QVector3D velocity;
    QVector3D force;
    QVector3D position;
    QVector3D positionAnt;
};

#endif // MESHSPHERE_H
