#version 130
uniform vec4 ambientProduct[2];
uniform vec4 diffuseProduct[2];
uniform vec4 specularProduct[2];
uniform vec2 phaseClouds;
uniform float shininess;
uniform int hasTexture;
in vec2 fTextCoord;
in vec3 N_frag;
in vec3 L_frag[2];
in vec3 E_frag;

uniform  sampler2D  texture1;
uniform  sampler2D  texture2;

out vec4 myfragcolor;

vec4 light(){

    vec3 N = normalize(N_frag);

    vec3 E = normalize(E_frag);
    vec4 diffuse =vec4(0.0,0.0,0.0,0.0);
    vec4 ambient =vec4(0.0,0.0,0.0,0.0);
    vec4 specular =vec4(0.0,0.0,0.0,0.0);
    for(int i=0;i<2;i++){
        vec3 L = normalize(L_frag[1]);
        vec3 R = normalize(2.0 * dot(L, N) * N - L);
        float NdotL = dot(N, L);
        float Ks = (NdotL < 0.0) ? 0.0 : pow(max(dot(R, E), 0.0),shininess);
        if(hasTexture==0){
           float Kd = max(NdotL , 0.0);
           diffuse =diffuse+( Kd * diffuseProduct[i]);
           ambient =ambient+ ambientProduct[i];
        }
        specular =specular+ max(Ks * specularProduct[i] ,0.0);

    }
    vec3 sumColor=vec3(0,0,0);
    if(hasTexture==2){
         vec2 fTextCoordIntern=fTextCoord;
        sumColor=texture2D(texture1,fTextCoordIntern).rgb+(texture2D(texture2,fTextCoord+phaseClouds ).rgb/2);
    }
    else if(hasTexture==1)
        sumColor=texture2D(texture1,fTextCoord).rgb;
    else{
        sumColor=(ambient + diffuse).rgb;
    }
    return vec4(sumColor+specular.rgb, 1.0);
}
void main(void)
{

    vec4 outColor=vec4(0,0,0,0);
    outColor=light();

    myfragcolor=outColor;
}

