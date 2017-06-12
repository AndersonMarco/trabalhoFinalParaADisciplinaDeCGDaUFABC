#ifndef MESH_H
#define MESH_H
#include <QString>
#include<QStringList>
#include <QVector4D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QImage>
#include<math.h>
#include "material.h"

class Mesh
{
public:
    Mesh(QString qStringMesh);
    Mesh();    
    ~Mesh();
    void rotate(QQuaternion rot);
    void loadtexture(QImage textureImg);
    void loadtexture2(QImage textureImg);
    Material material;
    QMatrix4x4 *modelView;


    QOpenGLBuffer *vboVertices;
    QOpenGLBuffer *vboNormalVertices;
    QOpenGLBuffer *vboIndices;
    QOpenGLBuffer *vboTexCoords;
    unsigned int numVertices;
    unsigned int numFaces;
    QOpenGLTexture * texture1;
    QOpenGLTexture * texture2;


protected:
    QVector3D normal(QVector3D vectorA, QVector3D vectorB, QVector3D vectorc);
    void calculateNormalToFaces();
    double invdiag;
    QVector3D midpoint;
    unsigned int * indices;
    QVector3D *normalToVertice;
    QVector4D * qVector4DVertices;
    QVector2D *texCoords;
    void indicesAndVerticesToVboObjects();
    void genTexCoordsCylinder();
};

#endif // MESH_H
