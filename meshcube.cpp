#include "meshcube.h"

MeshCube::MeshCube()
{
    force=QVector3D(0,0,0);
    velocity=QVector3D(0,0,0);
    numVertices=8;
    numFaces=6*2;
    indices= new unsigned int[numFaces*3];
    qVector4DVertices= new QVector4D[numVertices];
    qVector4DVertices[0]=QVector4D(-0.2, -0.2,  0.2,1);
    qVector4DVertices[1]=QVector4D(-0.2, -0.2, -0.2,1);
    qVector4DVertices[2]=QVector4D( 0.2, -0.2, -0.2,1);
    qVector4DVertices[3]=QVector4D( 0.2, -0.2,  0.2,1);
    qVector4DVertices[4]=QVector4D(-0.2,  0.2,  0.2,1);
    qVector4DVertices[5]=QVector4D(-0.2,  0.2, -0.2,1);
    qVector4DVertices[6]=QVector4D( 0.2,  0.2, -0.2,1);
    qVector4DVertices[7]=QVector4D( 0.2,  0.2,  0.2,1);

    indices[0] =0;indices[1] =1;indices[2] =3;
    indices[3] =4;indices[4] =7;indices[5] =5;
    indices[6] =0;indices[7] =4;indices[8] =1;
    indices[9] =1;indices[10]=5;indices[11]=6;
    indices[12]=2;indices[13]=6;indices[14]=3;
    indices[15]=4;indices[16]=0;indices[17]=3;
    indices[18]=1;indices[19]=2;indices[20]=3;
    indices[21]=7;indices[22]=6;indices[23]=5;
    indices[24]=4;indices[25]=5;indices[26]=1;
    indices[27]=2;indices[28]=1;indices[29]=6;
    indices[30]=6;indices[31]=7;indices[32]=3;
    indices[33]=7;indices[34]=4;indices[35]=3;   
    calculateNormalToFaces();
    indicesAndVerticesToVboObjects();
    genTexCoordsCylinder();
}
void MeshCube::setPosition(QVector3D position){
    modelView->setToIdentity();
    modelView->translate(position);
    this->position=position;
}
void MeshCube::calculatePosition(){
    if(haveForceOrVelocity()==false) return;
    positionAnt=position;
    QVector3D friction =velocity*0.7;
    force=force-friction;
    if(position.y()>0.2){
        force.setY(-0.4+force.y());
    }


    velocity=velocity+(force*0.1);
    position=position+(velocity*0.1);


    if(position.y()<=0.2){
        position.setY(0.2);
        force.setY(0);
        velocity.setY(0);
    }
    if(force.length()<0.001 && velocity.length()<0.01){

        velocity=QVector3D(0,0,0);
    }
    force=QVector3D(0,0,0);

    setPosition(position);
}
bool MeshCube::haveForceOrVelocity(){
    if(position.y()>0.2001) return true;
    if(force.length()<0.001 && velocity.length()<0.01){
        positionAnt=position;
        return false;
    }
    return true;
}
QVector3D MeshCube::getPosition(){
    return this->position;
}

