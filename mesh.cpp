#include "mesh.h"

Mesh::Mesh()
{
    modelView= new QMatrix4x4 ();
    modelView->setToIdentity();
    texCoords=NULL;
    texture1=NULL;
    texture2=NULL;

    vboTexCoords=NULL;
}

Mesh::~Mesh(){    
    vboVertices->release();
    delete vboVertices;

    vboNormalVertices->release();
    delete vboNormalVertices;

    vboIndices->release();
    delete vboIndices;

    vboTexCoords->release();
    delete vboTexCoords;

    delete indices;
    delete qVector4DVertices;
    delete modelView;
    delete normalToVertice;
    if(texCoords) delete texCoords;
    if(texture1) delete texture1;
    if(texture2) delete texture2;


}
QVector3D Mesh::normal(QVector3D vectorA, QVector3D vectorB, QVector3D vectorC){
    QVector3D aMinusB;
    QVector3D cMinusA;

    aMinusB.setX(vectorB.x()-vectorA.x());
    aMinusB.setY(vectorB.y()-vectorA.y());
    aMinusB.setZ(vectorB.z()-vectorA.z());

    cMinusA.setX(vectorC.x()-vectorA.x());
    cMinusA.setY(vectorC.y()-vectorA.y());
    cMinusA.setZ(vectorC.z()-vectorA.z());
    QVector3D ret=aMinusB.crossProduct(aMinusB,cMinusA).normalized();

    return ret;
}
void Mesh::calculateNormalToFaces(){
    normalToVertice =new QVector3D[numVertices];
    int ind=0;
    for (int i=0;i<numFaces;i++){
       QVector3D a= qVector4DVertices[indices[3*i]].toVector3D();
       QVector3D b= qVector4DVertices[indices[3*i+1]].toVector3D();
       QVector3D c= qVector4DVertices[indices[3*i+2]].toVector3D();
       QVector3D norm=QVector3D::normal(a-b,a-c);
       normalToVertice[indices[3*i]]+=norm;
       normalToVertice[indices[3*i+1]]+=norm;
       normalToVertice[indices[3*i+2]]+=norm;

    }
    for (int i=0;i<numVertices;i++){
        normalToVertice[i].normalize();
    }

}
void Mesh::rotate(QQuaternion rot){
      modelView->setToIdentity();
      //initialize matrix

      modelView->scale(invdiag ,invdiag ,invdiag);
      //second transformation to be applied

      modelView->translate(-midpoint);
      modelView->rotate(rot);
}
void  Mesh::indicesAndVerticesToVboObjects(){
    vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertices ->create();
    vboVertices ->bind();
    vboVertices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertices ->allocate(qVector4DVertices ,numVertices *sizeof(QVector4D));

    vboNormalVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboNormalVertices->create();
    vboNormalVertices->bind();
    vboNormalVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboNormalVertices->allocate(normalToVertice , numVertices *sizeof(QVector3D));

    vboIndices =new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vboIndices ->create();
    vboIndices ->bind();
    vboIndices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboIndices ->allocate(indices , numFaces * 3 *sizeof(unsigned int));
}
void Mesh::genTexCoordsCylinder(){
    if(texCoords)delete []texCoords;
    if(vboTexCoords){
        vboTexCoords->release();
        delete vboTexCoords;
    }
    texCoords=new QVector2D[numVertices];
    //Compute minimum and maximum values
    float fmax = 1000000000.0f;
    float minz = fmax;
    float maxz = -fmax;
    for(int i=0; i<numVertices; ++i){
        if(qVector4DVertices[i].z() < minz) minz = qVector4DVertices[i].z();
        if(qVector4DVertices[i].z() > maxz) maxz = qVector4DVertices[i].z();
    }
    texCoords =new QVector2D[numVertices];
    for(int i=0; i<numVertices; ++i){
        //https://en.wikipedia.org/wiki/Atan2
        float s = (atan2(qVector4DVertices[i].y(),qVector4DVertices[i].x())+M_PI)/(2*M_PI);       
        float t = 1.0-((qVector4DVertices[i].z() - minz)/(maxz -minz));
        float tx1 = atan2(qVector4DVertices[i].y(),qVector4DVertices[i].x()) / (2* M_PI) + 0.5;
        float ty1 = asin(-qVector4DVertices[i].z()) / M_PI + .5;
        texCoords[i] = QVector2D(tx1,ty1);
    }
    vboTexCoords= new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTexCoords->create();
    vboTexCoords->bind();
    vboTexCoords->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTexCoords->allocate(texCoords ,numVertices *sizeof(QVector2D));
}
void Mesh::loadtexture(QImage textureImg){
    if(texture1) delete texture1;

    texture1= new QOpenGLTexture(textureImg);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);
    texture1->setMinificationFilter(QOpenGLTexture::Linear);
    texture1->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::MirroredRepeat);
    texture1->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::MirroredRepeat);
}
void Mesh::loadtexture2(QImage textureImg){
    if(texture2) delete texture2;

    texture2= new QOpenGLTexture(textureImg);
    texture2->setMagnificationFilter(QOpenGLTexture::Linear);
    texture2->setMinificationFilter(QOpenGLTexture::Linear);
    texture2->setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::MirroredRepeat);
    texture2->setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::MirroredRepeat);
}
