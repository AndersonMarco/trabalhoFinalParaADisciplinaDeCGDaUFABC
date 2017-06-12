#ifndef QTHREADPHYSICSMESHCUBE_H
#define QTHREADPHYSICSMESHCUBE_H
#include <QThread>
#include "openglwidget.h"
#include "meshsphere.h"
class OpenGLWidget;
class QThreadPhysicsMeshCube : public QThread
{
private:
    OpenGLWidget* openGLWidget;
    MeshSphere **meshCube;
    int sizeMeshCube;
public:
    QThreadPhysicsMeshCube(OpenGLWidget* openGLWidget,MeshSphere **meshCube,int sizeMeshCube);
    void run();
    float calculateDeformationInFrameBufferImage();

};

#endif // QTHREADPHYSICSMESHCUBE_H
