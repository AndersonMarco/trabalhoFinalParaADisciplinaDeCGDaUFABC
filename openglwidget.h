#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QVector4D>
#include <QVector3D>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include "fileoperationsfromresource.h"
#include "mesh.h"
#include "meshcube.h"
#include "meshsphere.h"
#include "meshfromfile.h"
#include "camera.h"
#include "cameranorotation.h"
#include "material.h"
#include "light.h"
#include "qthreadphysicsmeshcube.h"

class QThreadPhysicsMeshCube;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:    
    explicit OpenGLWidget(QWidget *parent = 0);
    void createShaders();
    void destroyShaders();
    void loadMesh(Mesh *mesh);
    ~OpenGLWidget();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void repaintFromExternalObject();

    QTimer timer;

public slots:
    void slotKeyPress(int key);
    void slotMoveToFront(double scale);
    void slotMoveToBack(double scale);
    void slotMoveToRight(double scale);
    void slotMoveToLeft(double scale);
    void slotUpdateClouds();
    void slotRotateAxisXInCamera(double scale);
    void slotRotateAxisYInCamera(double scale);
    void slotJoystickButtonPress( bool state,int button);

signals:


protected:

    void initializeGL();
    void resizeGL(int width ,int height);
    void paintGL();


private:
    void holdMeshEvent(int idMeshHold);
    void releaseMeshEvent();
    void paintGLMesh(Mesh *mesh,int calcGravity);
    void paintGLMesh(Mesh *mesh, Camera camera,int calcGravity);
    void paintFloor();
    void paintFloor(Camera camera);
    void paintCanvas(QImage img);
    void paintCanvas(GLuint texture);
    QOpenGLShader *vertexShader;
    QOpenGLShader *fragmentShader;
    QOpenGLShaderProgram *shaderProgramMesh;
    QOpenGLShaderProgram *shaderProgramFloor;
    QOpenGLShaderProgram *shaderProgramPaintScene;
    QMatrix4x4  projectionMatrix;
    QMatrix4x4  cameraMat;
    Camera camera;

    Mesh *mesh;
    MeshSphere **meshSphere;
    QThreadPhysicsMeshCube *qThreadPhysicsMeshCube;
    QVector2D phaseClouds;
    QOpenGLFramebufferObjectFormat fboFormat;

    bool openglIsReady;
    int width;
    int height;
    int sizeOfMeshSphere;
    int minIdMeshCube;
    void holdMeshCube();

};

#endif // OPENGLWIDGET_H
