#version 130

out vec4 myfragcolor;
in vec2 v_texCoord;
uniform vec3 positionOfballs[6];
uniform int sizeOfBalls;
vec4 colorRegion(vec2 coord){
    int x=int(coord.x*10);
    int y=int(coord.y*10);
    vec4 color=vec4(0.3,0.3,0.3,1);
    if(x%2==0){
       if(y%2==0)
          color=vec4(0.8,0.8,0.8,1);
    }
    else{
       if(y%2==1)
          color=vec4(0.8,0.8,0.8,1);
    }
    return color;
}
vec4 colorRegionSuav(vec2 texCoord){
    vec2 inc1=vec2(1,0)*0.0004;
    vec2 inc2=vec2(0,1)*0.0004;
    vec2 inc3=vec2(1,1)*0.0004;
    vec2 inc4=vec2(1,-1)*0.0004;
    vec4 ret=colorRegion(texCoord)+
             colorRegion(texCoord+inc1)+colorRegion(texCoord-inc1)+
             colorRegion(texCoord+inc2)+colorRegion(texCoord-inc2)+
             colorRegion(texCoord+inc3)+colorRegion(texCoord-inc3)+
             colorRegion(texCoord+inc4)+colorRegion(texCoord-inc4);

    return ret/8;
}
vec4 colorOfBall(int ball){
    if(ball==0) return vec4(1,0,0,1);
    if(ball==1) return vec4(0,1,0,1);
    if(ball==2) return vec4(0,0,1,1);
    if(ball==3) return vec4(1,0,1,1);
    if(ball==4) return vec4(1,1,0,1);
    return vec4(0,1,1,1);
}
void main(void)
{
    //myfragcolor=color_out;

    vec2 regionOfPlane=vec2((v_texCoord.x*20)-10,(v_texCoord.y*20)-10);
    vec4 sumColor=vec4(0,0,0,1);

    for(int i=0;i<sizeOfBalls;i++){
        vec2 posi=vec2(-positionOfballs[i].z,-positionOfballs[i].x);
        float d=(length(posi-regionOfPlane));
        float dt=(1.0-d)*0.7;
        if(d<1){

            sumColor=sumColor+(colorOfBall(i)*dt);
        }
    }
    sumColor=colorRegionSuav(v_texCoord)+sumColor;
    if(sumColor.x>1) sumColor.x=1;
    if(sumColor.y>1) sumColor.y=1;
    if(sumColor.z>1) sumColor.z=1;

    myfragcolor=vec4(sumColor.rgb,1);

}

