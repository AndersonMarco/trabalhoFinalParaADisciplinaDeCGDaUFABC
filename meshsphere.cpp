#include "meshsphere.h"

MeshSphere::MeshSphere()
{
    QString pathSphereMesh=":/off_models/sphere.off";
    QString qStringMesh= FileOperationsFromResource::readFile(pathSphereMesh);
    QStringList list1 = qStringMesh.split("\n");
    numVertices=(list1[1].split(" ")[0]).toInt();
    numFaces=(list1[1].split(" ")[1]).toInt();
    qVector4DVertices= new QVector4D[numVertices];
    indices= new unsigned int[numFaces*3];
    double minx=-1;
    double miny=-1;
    double minz=-1;
    double maxx=-1;
    double maxy=-1;
    double maxz=-1;
    double b;
    for(int i=0;i<numVertices;i++){
        QStringList vertice=list1[i+2].split(" ");
        if(i==0){
            minx=vertice[0].toDouble();
            miny=vertice[1].toDouble();
            minz=vertice[2].toDouble();
            maxx=vertice[0].toDouble();
            maxy=vertice[1].toDouble();
            maxz=vertice[2].toDouble();
        }
        if(minx>vertice[0].toDouble())minx=vertice[0].toDouble();
        if(miny>vertice[1].toDouble())miny=vertice[1].toDouble();
        if(minz>vertice[2].toDouble())minz=vertice[2].toDouble();
        if(maxx<vertice[0].toDouble())maxx=vertice[0].toDouble();
        if(maxy<vertice[1].toDouble())maxy=vertice[1].toDouble();
        if(maxz<vertice[2].toDouble())maxz=vertice[2].toDouble();

        qVector4DVertices[i]=QVector4D(vertice[0].toDouble(),vertice[1].toDouble(),vertice[2].toDouble(),1.0);
    }
    QVector3D max= QVector3D(maxx,maxy,maxz);
    QVector3D min= QVector3D(minx,miny,minz);
    midpoint = QVector3D(0,-0.5,0);
    invdiag = (1 / (max - min).length())*0.8;

    //initialize matrix

    modelView->scale(invdiag ,invdiag ,invdiag);
    //second transformation to be applied
    modelView->translate(-midpoint);
    //first transformation to be applied
    /*for(int i=0;i<numVertices;i++){
        qVector4DVertices[i] = (qVector4DVertices[i] - midpoint) * invdiag;
        qVector4DVertices[i].setW(1);
    }*/
    for(int i=0;i<numFaces;i++){
        QStringList indice=list1[i+2+numVertices].split(" ");
        indices[i*3]=indice[1].toInt();
        indices[i*3+1]=indice[2].toInt();
        indices[i*3+2]=indice[3].toInt();
    }
    calculateNormalToFaces();
    indicesAndVerticesToVboObjects();
    genTexCoordsCylinder();
}
void MeshSphere::calculatePosition(){
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
bool MeshSphere::haveForceOrVelocity(){
    if(position.y()>0.2001) return true;
    if(force.length()<0.001 && velocity.length()<0.01){
        positionAnt=position;
        return false;
    }
    return true;
}
QVector3D MeshSphere::getPosition(){
    return this->position;
}
void MeshSphere::setPosition(QVector3D position){
    modelView->setToIdentity();
    modelView->translate(position);
    modelView->scale(invdiag ,invdiag ,invdiag);
    modelView->rotate(-90,QVector3D(1,0,0));
    this->position=position;
}
