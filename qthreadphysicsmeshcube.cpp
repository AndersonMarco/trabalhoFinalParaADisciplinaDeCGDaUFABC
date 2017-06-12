#include "qthreadphysicsmeshcube.h"

QThreadPhysicsMeshCube::QThreadPhysicsMeshCube(OpenGLWidget* openGLWidget,MeshSphere **meshCube, int sizeMeshCube)
{
    this->openGLWidget=openGLWidget;
    this->meshCube=meshCube;
    this->sizeMeshCube=sizeMeshCube;
}
float QThreadPhysicsMeshCube::calculateDeformationInFrameBufferImage(){
    float ret=0.0;
    for(int i=0;i<sizeMeshCube;i++)
            ret=ret+meshCube[i]->velocity.length();
    return ret*10;
}
void QThreadPhysicsMeshCube::run(){
    for(int i=0;i<sizeMeshCube;i++)
        meshCube[i]->haveForceOrVelocity();
    int calculos=0;
    bool breakWhile=false;
    while(true){
        msleep(30);
        for(int i=0;i<sizeMeshCube;i++)
            meshCube[i]->calculatePosition();
        openGLWidget->repaintFromExternalObject();

        //detect colision==================================================
        for(int i=0;i<sizeMeshCube;i++){
            for(int j=0;j<sizeMeshCube;j++){
                //if(i!=j && qAbs(meshCube[i]->getPosition().x()-meshCube[j]->getPosition().x()) <0.2 && qAbs(meshCube[j]->getPosition().z()-meshCube[j]->getPosition().z()) <0.2 && qAbs(meshCube[j]->getPosition().y()-meshCube[j]->getPosition().y()) <0.2){
                if(i!=j && (meshCube[i]->getPosition()-meshCube[j]->getPosition()).length() <0.50 ){
                    QVector3D vTemp=meshCube[i]->velocity;
                    meshCube[i]->velocity=meshCube[j]->velocity*1*(1+(meshCube[j]->position.y()-0.2));;
                    meshCube[j]->velocity=vTemp*1*(1+(meshCube[i]->position.y()-0.2));
                    meshCube[j]->position=meshCube[j]->positionAnt;
                    meshCube[i]->position=meshCube[i]->positionAnt;
                    calculos++;
                    if(calculos>160){
                        for(int k=0;k<sizeMeshCube;k++){
                            meshCube[k]->position.setY(0.2);
                            meshCube[k]->positionAnt.setY(0.2);
                            meshCube[k]->velocity.setX(0);
                            meshCube[k]->velocity.setY(0);
                            meshCube[k]->velocity.setZ(0);
                        }
                        openGLWidget->repaintFromExternalObject();
                        breakWhile=true;
                        break;
                    }

                }                
            }
            if(breakWhile) break;
        }
        //end==============================================================
        //checking if the thread will stop
        bool allMeshCubeNotHaveForceAndVelocity=true;

        for(int i=0;i<sizeMeshCube;i++){
            if(meshCube[i]->haveForceOrVelocity()==true){
                allMeshCubeNotHaveForceAndVelocity=false;
                break;
            }            
        }        
        if(allMeshCubeNotHaveForceAndVelocity) break;
        //end===========================================================

    }
}
