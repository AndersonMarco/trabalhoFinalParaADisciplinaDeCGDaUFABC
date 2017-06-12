#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    sizeOfMeshSphere=6;
    meshSphere =new MeshSphere*[sizeOfMeshSphere];
    vertexShader=NULL;
    fragmentShader=NULL;
    shaderProgramMesh=NULL;
    shaderProgramFloor=NULL;
    mesh=NULL;
    shaderProgramPaintScene=NULL;
    minIdMeshCube=-1;
    for(int i=0;i<sizeOfMeshSphere;i++)
        meshSphere[i]=NULL;

    qThreadPhysicsMeshCube=NULL;
    phaseClouds=QVector2D(0,0);
    openglIsReady=false;

}
OpenGLWidget::~OpenGLWidget(){
    destroyShaders();
    delete mesh;
    for(int i=0;i<6;i++){
        delete meshSphere[i];
    }
    delete meshSphere;
    if(qThreadPhysicsMeshCube!=NULL) delete qThreadPhysicsMeshCube;
    if(shaderProgramPaintScene!=NULL) delete shaderProgramPaintScene;
}


void OpenGLWidget::destroyShaders(){


    if(vertexShader)delete vertexShader;
    vertexShader=NULL;
    if(fragmentShader)delete fragmentShader;
    fragmentShader=NULL;
    if(shaderProgramMesh){
        shaderProgramMesh->release();
         delete shaderProgramMesh;
         shaderProgramMesh=NULL;
    }
    if(shaderProgramFloor){
        shaderProgramFloor->release();
        delete shaderProgramFloor;
        shaderProgramFloor=NULL;
    }
}

void OpenGLWidget::createShaders(){
    destroyShaders();
    QString vertexShaderFile=":/mesh.vert";
    QString fragmentShaderFile=":/mesh.frag";
    QString vertexShaderFloorFile=":/floor.vert";
    QString fragmentShaderFloorFile=":/floor.frag";
    QString vertexShaderPaintSceneFile=":/paintscene.vert";
    QString fragmentShaderPaintSceneFile=":/paintscene.frag";

    shaderProgramPaintScene=new QOpenGLShaderProgram;
    shaderProgramFloor = new QOpenGLShaderProgram;
    shaderProgramMesh = new QOpenGLShaderProgram;


    shaderProgramMesh->addShaderFromSourceCode(QOpenGLShader::Vertex,FileOperationsFromResource::readFile(vertexShaderFile));
    shaderProgramMesh->addShaderFromSourceCode(QOpenGLShader::Fragment,FileOperationsFromResource::readFile(fragmentShaderFile));
    shaderProgramFloor->addShaderFromSourceCode(QOpenGLShader::Vertex,FileOperationsFromResource::readFile(vertexShaderFloorFile));
    shaderProgramFloor->addShaderFromSourceCode(QOpenGLShader::Fragment,FileOperationsFromResource::readFile(fragmentShaderFloorFile));
    shaderProgramPaintScene->addShaderFromSourceCode(QOpenGLShader::Vertex,FileOperationsFromResource::readFile(vertexShaderPaintSceneFile));
    shaderProgramPaintScene->addShaderFromSourceCode(QOpenGLShader::Fragment,FileOperationsFromResource::readFile(fragmentShaderPaintSceneFile));
    if(!shaderProgramMesh->link()){
        qWarning() << shaderProgramMesh->log() << endl;
    }

    if(!shaderProgramFloor->link()){
        qWarning() << shaderProgramFloor->log() << endl;
    }

    if(!shaderProgramPaintScene->link()){
        qWarning() << shaderProgramPaintScene->log() << endl;
    }

}

void OpenGLWidget::initializeGL(){
    //fboFormat.setSamples(16);
    fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    this->createShaders();
    for(int i=0;i<sizeOfMeshSphere;i++)
        meshSphere[i]=new MeshSphere();

    if(sizeOfMeshSphere>0){
        meshSphere[0]->setPosition(QVector3D(5.8,0.2,0.2));
        meshSphere[0]->loadtexture(QImage(":/img/earth.jpg"));
        meshSphere[0]->loadtexture2(QImage(":/img/clouds.jpg"));
        meshSphere[0]->material.ambient=QVector4D(0.3,0.3,0.3,1);
        meshSphere[0]->material.diffuse=QVector4D(0.4,0.4,0.4,1);
        meshSphere[0]->material.specular=QVector4D(0.9,0.9,0.9,1);
    }
    if(sizeOfMeshSphere>1){
        meshSphere[1]->setPosition(QVector3D(-0.8,0.2,0.6));
        meshSphere[1]->loadtexture(QImage(":/img/mars.jpg"));
        meshSphere[1]->material.ambient=QVector4D(0.3,0.3,0.3,1);
        meshSphere[1]->material.diffuse=QVector4D(1,1,1,1);
        meshSphere[1]->material.specular=QVector4D(0.9,0.9,0.9,1);
    }
    if(sizeOfMeshSphere>2){
        meshSphere[2]->setPosition(QVector3D(3.3,0.2,4.3));
        meshSphere[2]->loadtexture(QImage(":/img/venus.jpg"));
        meshSphere[2]->material.ambient=QVector4D(0.3,0.3,0.3,1);
        meshSphere[2]->material.diffuse=QVector4D(1,1,1,1);
        meshSphere[2]->material.specular=QVector4D(0.9,0.9,0.9,1);
    }

    if(sizeOfMeshSphere>3){
        meshSphere[3]->setPosition(QVector3D(-6.5,0.2,-2.2));
        meshSphere[3]->material.diffuse=QVector4D(0.6,.7,.7,1);
        meshSphere[3]->material.ambient=QVector4D(0.2,.4,.4,1);
    }
    if(sizeOfMeshSphere>4){
        meshSphere[4]->setPosition(QVector3D(4.2,0.2,-4.2));
        meshSphere[4]->material.diffuse=QVector4D(0.7,.7,.6,1);
        meshSphere[4]->material.ambient=QVector4D(0.4,.4,.2,1);

    }
    if(sizeOfMeshSphere>5) meshSphere[5]->setPosition(QVector3D(3.7,0.2,0.2));
    mesh=new MeshFromFile(FileOperationsFromResource::readFile( ":/off_models/camel.off"));



    initializeOpenGLFunctions();
    openglIsReady=true;

}
void OpenGLWidget::resizeGL(int w, int h){
    width=w;
    height=h;
    glViewport(0, 0, w, h);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0,static_cast <qreal >(w) /static_cast<qreal >(h), 0.1,20.0);
    camera.resizeViewport(w,h);

}
void OpenGLWidget::paintGL(){


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);    
    QOpenGLFramebufferObject fbo(width,height,fboFormat);

    fbo.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int cont=0;cont<sizeOfMeshSphere;cont++){
         paintGLMesh(meshSphere[cont],0);
    }

    if(mesh!=NULL){
        paintGLMesh(mesh,1);
    }
    paintFloor();              

    QOpenGLFramebufferObject::bindDefault();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_MULTISAMPLE);
    glDisable(GL_LINE_SMOOTH);
    makeCurrent();

    paintCanvas(fbo.texture());
    fbo.release();

}
void OpenGLWidget::repaintFromExternalObject(){


    update();
}
void OpenGLWidget::paintFloor(){
    paintFloor(camera);
}
void OpenGLWidget::paintFloor(Camera camera){
    QMatrix4x4 modelViewArgumentToShader;
    modelViewArgumentToShader.setToIdentity();
    modelViewArgumentToShader.lookAt(camera.eye, camera.at, camera.up);

    shaderProgramFloor->bind();


    //define floor=====================================================
    QVector4D *vertices= new QVector4D[4];
    vertices[0]=QVector4D(-10.0, 0.0,  10.0,1);
    vertices[1]=QVector4D(-10.0, 0.0, -10.0,1);
    vertices[2]=QVector4D( 10.0, 0.0, -10.0,1);
    vertices[3]=QVector4D( 10.0, 0.0,  10.0,1);


    unsigned int  *indices = new unsigned int[2*3];
    indices[0]=0; indices[1]=1; indices[2]=2;
    indices[3]=2; indices[4]=3; indices[5]=0;


    QVector2D *textureCoordinate=new QVector2D[4];
    textureCoordinate[3]= QVector2D(0,0);
    textureCoordinate[2]=QVector2D(1,0);
    textureCoordinate[1]=QVector2D(1,1);
    textureCoordinate[0]=QVector2D(0,1);
    //end==============================================================


    //transfer floor to gpu============================================
    QOpenGLBuffer *vboIndices =new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vboIndices ->create();
    vboIndices ->bind();
    vboIndices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboIndices ->allocate(indices , 2 * 3 *sizeof(unsigned int));
    vboIndices ->release();

    QOpenGLBuffer *vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertices ->create();
    vboVertices ->bind();
    vboVertices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertices ->allocate(vertices , 4 *sizeof(QVector4D));
    shaderProgramFloor->enableAttributeArray("vPosition");//vertices is linked with a_position
    shaderProgramFloor->setAttributeBuffer("vPosition",GL_FLOAT, 0,4,0);//vertices is linked with a_position
    vboVertices->release();
    //end===============================================================
    //transfer textureCoordinate to gpu=================================

    QOpenGLBuffer *vboTextureCoordinate =new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTextureCoordinate->create();

    vboTextureCoordinate->bind();
    vboTextureCoordinate->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTextureCoordinate->allocate(textureCoordinate , 4*sizeof(QVector2D));
    shaderProgramFloor->enableAttributeArray("a_texCoord");
    shaderProgramFloor->setAttributeBuffer("a_texCoord",GL_FLOAT, 0,2,0);

    //end===============================================================
    shaderProgramFloor->setUniformValue("mModelView",modelViewArgumentToShader);
    shaderProgramFloor->setUniformValue("mProjection",projectionMatrix);
    QVector3D positionOfballs[6];
    for(int cont=0;cont<sizeOfMeshSphere;cont++)
        positionOfballs[cont]=meshSphere[cont]->position;
    shaderProgramFloor->setUniformValueArray("positionOfballs",positionOfballs,sizeOfMeshSphere);
    shaderProgramFloor->setUniformValue("sizeOfBalls",sizeOfMeshSphere);
    vboIndices->bind();
    glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,0);
    vboIndices->release();
    delete vboVertices;
    delete vboIndices;
    delete indices;
    delete vertices;
    delete textureCoordinate;
    shaderProgramFloor->release();


}
void  OpenGLWidget::paintGLMesh(Mesh *mesh,int calcGravity){
    paintGLMesh(mesh,camera,calcGravity);
}
void  OpenGLWidget::paintGLMesh(Mesh *mesh, Camera camera,int calcGravity){
    if(mesh==NULL) return;
    int numOfLight=2;
    QVector4D lightPositions[2];
    Light light[2];
    light[0].position= QVector4D(-10,10,10, 1.0);
    light[1].position= QVector4D(0,10,0, 1.0);
    for(int i=0;i<numOfLight;i++){
        lightPositions[i]=light[i].position;
    }

    QVector4D ambientProduct[2];
    QVector4D diffuseProduct[2];
    QVector4D specularProduct[2];

    for(int i=0;i<numOfLight;i++){
        ambientProduct[i] = light[i].ambient * mesh->material.ambient;
        diffuseProduct[i] = light[i].diffuse *  mesh->material.diffuse;
        specularProduct[i] = light[i].specular *  mesh->material.specular;
    }


    QMatrix4x4 mModelView;
    mModelView.setToIdentity();

    mModelView.lookAt(camera.eye, camera.at, camera.up);
    QMatrix4x4 modelViewMesh=(*mesh->modelView);
    mModelView=mModelView*modelViewMesh;
    mModelView;
    shaderProgramMesh->bind();



    mesh->vboVertices->bind();
    shaderProgramMesh->enableAttributeArray("vPosition");
    shaderProgramMesh->setAttributeBuffer("vPosition",GL_FLOAT, 0,4,0);

    mesh->vboNormalVertices->bind();
    shaderProgramMesh->enableAttributeArray("normalVertice");
    shaderProgramMesh->setAttributeBuffer("normalVertice",GL_FLOAT, 0,3,0);

    if(mesh->texture1 && mesh->vboTexCoords){
        glActiveTexture(GL_TEXTURE0);
        mesh->texture1->bind();
        glUniform1i(glGetUniformLocation(shaderProgramMesh->programId(), "texture1"), 0);
    }
    if(mesh->texture2 && mesh->vboTexCoords){
        glActiveTexture(GL_TEXTURE1);
        mesh->texture2->bind();
        glUniform1i(glGetUniformLocation(shaderProgramMesh->programId(), "texture2"), 1);
    }

    if(mesh->vboTexCoords){
        mesh->vboTexCoords->bind();
        shaderProgramMesh->enableAttributeArray("vTextCoord");
        shaderProgramMesh->setAttributeBuffer("vTextCoord",GL_FLOAT, 0,2,0);
    }
    if(mesh->texture1 && mesh->texture2 && mesh->vboTexCoords){
        shaderProgramMesh->setUniformValue("hasTexture",2);
        shaderProgramMesh->setUniformValue("phaseClouds",phaseClouds);
    }
    else if(mesh->texture1 && mesh->vboTexCoords)
        shaderProgramMesh->setUniformValue("hasTexture",1);
    else
        shaderProgramMesh->setUniformValue("hasTexture",0);


    shaderProgramMesh->setUniformValue("normalMatrix",mModelView.normalMatrix());
    shaderProgramMesh->setUniformValue("mModelView",mModelView);
    shaderProgramMesh->setUniformValue("mProjection",projectionMatrix);

    shaderProgramMesh ->setUniformValueArray("lightPosition", lightPositions,numOfLight);


    shaderProgramMesh ->setUniformValueArray("ambientProduct",ambientProduct,numOfLight);
    shaderProgramMesh ->setUniformValueArray("diffuseProduct",diffuseProduct,numOfLight);
    shaderProgramMesh ->setUniformValueArray("specularProduct",specularProduct,numOfLight);
    shaderProgramMesh ->setUniformValue("shininess",static_cast<GLfloat >(mesh->material.shininess));

    QVector3D positionOfballs[6];
    for(int cont=0;cont<sizeOfMeshSphere;cont++)
        positionOfballs[cont]=meshSphere[cont]->position;
    shaderProgramMesh->setUniformValueArray("positionOfballs",positionOfballs,sizeOfMeshSphere);
    shaderProgramMesh->setUniformValue("calcGravity",calcGravity);
    shaderProgramMesh->setUniformValue("sizeOfBalls",sizeOfMeshSphere);



    mesh->vboIndices->bind();
    glDrawElements(GL_TRIANGLES,mesh->numFaces*3,GL_UNSIGNED_INT,0);
    mesh->vboIndices->release();
    mesh->vboVertices->release();
    mesh->vboNormalVertices->release();
    mesh->vboTexCoords->release();
    if(mesh->texture1) mesh->texture1->release();
    if(mesh->texture2) mesh->texture2->release();

    shaderProgramMesh->release();
}
void OpenGLWidget::paintCanvas(QImage img){
    //define canvas=====================================================
    QVector4D *vertices =new QVector4D[4];
    vertices[0]=  QVector4D(0,0,0,1);
    vertices[1]=  QVector4D((float)img.width(),0,0,1);
    vertices[2]=  QVector4D((float)img.width(),(float)img.height(),0,1);
    vertices[3]=  QVector4D(0,(float)img.height(),0,1);


    unsigned int  *indices = new unsigned int[2*3];
    indices[0]=0; indices[1]=1; indices[2]=2;
    indices[3]=2; indices[4]=3; indices[5]=0;


    QVector2D *textureCoordinate=new QVector2D[4];
    textureCoordinate[3]= QVector2D(0,0);
    textureCoordinate[2]=QVector2D(1,0);
    textureCoordinate[1]=QVector2D(1,1);
    textureCoordinate[0]=QVector2D(0,1);
    //end==============================================================

    shaderProgramPaintScene->bind();
    //canvas to gpu====================================================
    QOpenGLBuffer *vboIndices =new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vboIndices ->create();
    vboIndices ->bind();
    vboIndices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboIndices ->allocate(indices , 2 * 3 *sizeof(unsigned int));
    vboIndices ->release();

    QOpenGLBuffer *vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertices ->create();
    vboVertices ->bind();
    vboVertices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertices ->allocate(vertices , 4 *sizeof(QVector4D));
    shaderProgramPaintScene->enableAttributeArray("a_position");//vertices is linked with a_position
    shaderProgramPaintScene->setAttributeBuffer("a_position",GL_FLOAT, 0,4,0);//vertices is linked with a_position
    vboVertices->release();
    //end===============================================================

    //transfer textureCoordinate to gpu=================================
    QOpenGLBuffer *vboTextureCoordinate =new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTextureCoordinate->create();
    vboTextureCoordinate->bind();
    vboTextureCoordinate->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTextureCoordinate->allocate(textureCoordinate , 4*sizeof(QVector2D));
    shaderProgramPaintScene->enableAttributeArray("a_texCoord");
    shaderProgramPaintScene->setAttributeBuffer("a_texCoord",GL_FLOAT, 0,2,0);
    //end===============================================================

    glClear(GL_COLOR_BUFFER_BIT);
    QOpenGLTexture  texture(img);
    texture.setMagnificationFilter(QOpenGLTexture::Linear);
    texture.setMinificationFilter(QOpenGLTexture::Linear);
    texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::MirroredRepeat);
    texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::MirroredRepeat);
    glActiveTexture(GL_TEXTURE0);    
    texture.bind();
    glUniform1i(glGetUniformLocation(shaderProgramPaintScene->programId(), "texture1"), 0);



    QVector2D resolution(img.width(), img.height());
    shaderProgramPaintScene->setUniformValue("u_resolution",resolution);
    if(qThreadPhysicsMeshCube!=NULL) shaderProgramPaintScene->setUniformValue("frequency",qThreadPhysicsMeshCube->calculateDeformationInFrameBufferImage());
    else shaderProgramPaintScene->setUniformValue("frequency",0.0f);

    vboIndices->bind();
    glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,0);
    vboIndices->release();
    texture.release();
    vboTextureCoordinate->release();
    delete vboTextureCoordinate;
    delete vboVertices;
    delete vboIndices;
    delete textureCoordinate;
    delete indices;
    delete vertices;
}

void OpenGLWidget::paintCanvas(GLuint texture){
    //define canvas=====================================================
    QVector4D *vertices =new QVector4D[4];
    vertices[0]=  QVector4D(0,0,0,1);
    vertices[1]=  QVector4D((float)width,0,0,1);
    vertices[2]=  QVector4D((float)width,(float)height,0,1);
    vertices[3]=  QVector4D(0,(float)height,0,1);


    unsigned int  *indices = new unsigned int[2*3];
    indices[0]=0; indices[1]=1; indices[2]=2;
    indices[3]=2; indices[4]=3; indices[5]=0;


    QVector2D *textureCoordinate=new QVector2D[4];
    textureCoordinate[3]= QVector2D(0,0);
    textureCoordinate[2]=QVector2D(1,0);
    textureCoordinate[1]=QVector2D(1,1);
    textureCoordinate[0]=QVector2D(0,1);
    //end==============================================================

    shaderProgramPaintScene->bind();
    //canvas to gpu====================================================
    QOpenGLBuffer *vboIndices =new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    vboIndices ->create();
    vboIndices ->bind();
    vboIndices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboIndices ->allocate(indices , 2 * 3 *sizeof(unsigned int));
    vboIndices ->release();

    QOpenGLBuffer *vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboVertices ->create();
    vboVertices ->bind();
    vboVertices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboVertices ->allocate(vertices , 4 *sizeof(QVector4D));
    shaderProgramPaintScene->enableAttributeArray("a_position");//vertices is linked with a_position
    shaderProgramPaintScene->setAttributeBuffer("a_position",GL_FLOAT, 0,4,0);//vertices is linked with a_position
    vboVertices->release();
    //end===============================================================

    //transfer textureCoordinate to gpu=================================
    QOpenGLBuffer *vboTextureCoordinate =new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboTextureCoordinate->create();
    vboTextureCoordinate->bind();
    vboTextureCoordinate->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTextureCoordinate->allocate(textureCoordinate , 4*sizeof(QVector2D));
    shaderProgramPaintScene->enableAttributeArray("a_texCoord");
    shaderProgramPaintScene->setAttributeBuffer("a_texCoord",GL_FLOAT, 0,2,0);
    //end===============================================================

    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgramPaintScene->programId(), "texture1"), 0);



    QVector2D resolution(width, height);
    shaderProgramPaintScene->setUniformValue("u_resolution",resolution);
    if(qThreadPhysicsMeshCube!=NULL) shaderProgramPaintScene->setUniformValue("frequency",qThreadPhysicsMeshCube->calculateDeformationInFrameBufferImage());
    else shaderProgramPaintScene->setUniformValue("frequency",0.0f);

    vboIndices->bind();
    glDrawElements(GL_TRIANGLES,2*3,GL_UNSIGNED_INT,0);
    vboIndices->release();

    vboTextureCoordinate->release();
    delete vboTextureCoordinate;
    delete vboVertices;
    delete vboIndices;
    delete textureCoordinate;
    delete indices;
    delete vertices;
}
void OpenGLWidget::slotKeyPress(int key){

    if(key == Qt::Key_W){
        camera.toFront(1.0);
        holdMeshCube();
    }
    if(key == Qt::Key_S){
        camera.toBack(1.0);
        holdMeshCube();
    }
    if(key == Qt::Key_A){
        camera.toLeft(1.0);

        holdMeshCube();
    }
    if(key == Qt::Key_D){
        camera.toRight(1.0);

        holdMeshCube();
    }
    update();
}
void OpenGLWidget::slotMoveToFront(double scale){
    camera.toFront(scale);
    holdMeshCube();
    update();
}
void OpenGLWidget::slotMoveToBack(double scale){
    camera.toBack(scale);
    holdMeshCube();
    update();
}
void OpenGLWidget::slotMoveToLeft(double scale){
    camera.toLeft(scale);
    holdMeshCube();
    update();
}
void OpenGLWidget::slotMoveToRight(double scale){
    camera.toRight(scale);
    holdMeshCube();
    update();
}
void OpenGLWidget::slotRotateAxisXInCamera(double scale){
    camera.rotateAxisX(scale);
    holdMeshCube();
    update();
}
void OpenGLWidget::slotRotateAxisYInCamera(double scale){
    camera.rotateAxisY(scale);
    holdMeshCube();
    update();
}
void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{


    camera.mouseMove(event ->localPos());


    holdMeshCube();

    update();

}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    holdMeshEvent(-1);
}
void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    releaseMeshEvent();
}
void OpenGLWidget::holdMeshEvent(int idMeshHold){

    if(qThreadPhysicsMeshCube!=NULL && qThreadPhysicsMeshCube->isFinished()==false) return;

    minIdMeshCube=-1;
    double min=-1.0;
    if(idMeshHold<0){

        for(int i=0;i<sizeOfMeshSphere;i++){

            if((meshSphere[i]->getPosition()-camera.eye).length()<min || minIdMeshCube==-1){
                min=(meshSphere[i]->getPosition()-camera.eye).length();
                minIdMeshCube=i;
            }
        }

    }
    else
        minIdMeshCube=idMeshHold;
    holdMeshCube();
    update();
}
void OpenGLWidget::releaseMeshEvent(){
    qDebug() <<"ola cliquei";
    if(qThreadPhysicsMeshCube!=NULL){
        if(qThreadPhysicsMeshCube->isFinished()==false)
            return;
    }
    if(minIdMeshCube!=-1) meshSphere[minIdMeshCube]->force=-camera.getDirection()*100;



    if(qThreadPhysicsMeshCube!=NULL) delete qThreadPhysicsMeshCube;
    qThreadPhysicsMeshCube=new QThreadPhysicsMeshCube(this,meshSphere,sizeOfMeshSphere);
    qThreadPhysicsMeshCube->start();
    minIdMeshCube=-1;
    update();
}
void OpenGLWidget::slotJoystickButtonPress(bool state,int button){
    if(state){
        if(button==6)
            holdMeshEvent(-1);
       if(button<6) holdMeshEvent(button);

    }
    else{
        releaseMeshEvent();
    }
}
void OpenGLWidget::holdMeshCube(){
    if(minIdMeshCube>-1){
         qDebug() <<"ola cliquei 1";
        QVector3D posi=camera.eye;
        posi.setY(1);

        meshSphere[minIdMeshCube]->setPosition(posi-(camera.getDirection()*10));
    }

}
void OpenGLWidget::loadMesh(Mesh *mesh){
    makeCurrent();
    if(this->mesh){
        delete this->mesh;
    }

    this->mesh=mesh;

    update();
}
void OpenGLWidget::slotUpdateClouds(){
    if(openglIsReady){

        makeCurrent();
        if(phaseClouds.x()<40)
            phaseClouds.setX(phaseClouds.x()+(((double)rand())/100000000000.0));
        else
            phaseClouds.setX(phaseClouds.x()-(((double)rand())/100000000000.0));
        if(phaseClouds.y()<40)
            phaseClouds.setY(phaseClouds.y()+(((double)rand())/100000000000.0));

        else
            phaseClouds.setX(phaseClouds.x()-(((double)rand())/100000000000.0));

        update();
    }

}
