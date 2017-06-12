#version 130
in vec4 vPosition;
in vec3 normalVertice;
in vec2 vTextCoord;
out vec4 color_out;
out vec3 N_frag;
out vec3 L_frag[2];
out vec3 E_frag;
out vec2 fTextCoord;

uniform mat4 mModelView;
uniform mat3 normalMatrix;
uniform mat4 mProjection;
uniform vec4 lightPosition[2];
uniform int calcGravity;
uniform vec3 positionOfballs[6];
uniform int sizeOfBalls;
void light( vec4 eyePosition ){
    int sizeOfLight=2;
     N_frag = normalMatrix * abs(normalVertice);
     for(int i=0;i<sizeOfLight;i++){
        L_frag[i] = lightPosition[i].xyz - eyePosition.xyz;
     }
     E_frag = -eyePosition.xyz;

}
void main(void)
{

    vec3 vPositionLocal=vPosition.xyz;
    if(calcGravity==1){
        bool breakGravity=false;
        vec3 velocity=vec3(0,0,0);
        for(int j=0;j<90;j++){
            vec3 forceG=vec3(0,0,0);
            for(int i=0;i<sizeOfBalls;i++){
                float dist=length(positionOfballs[i]-vPositionLocal);
                if(dist<0.01){
                    breakGravity=true;
                    break;
                }
                dist=dist*dist;

                forceG=forceG+(((-vPositionLocal+positionOfballs[i])/dist));
            }

            velocity=velocity+(forceG*0.1);
            if(velocity.x>2) velocity.x=2;
            if(velocity.y>2) velocity.y=2;
            if(velocity.z>2) velocity.z=2;
            if(breakGravity) break;
            vPositionLocal=vPositionLocal+(velocity*0.001);

        }
        if(vPositionLocal.y<-2) vPositionLocal.y=-2;
    }

    fTextCoord=vTextCoord;
    vec4 eyePosition = mModelView * vec4(vPositionLocal,vPosition.w);
    light(eyePosition);
    vec4 posiVertice=eyePosition;
    gl_Position=mProjection*posiVertice;

}

