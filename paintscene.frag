#version 130
in vec2 v_texCoord;
out vec4 myfragcolor;
uniform sampler2D texture1;
uniform vec2 u_resolution;
uniform float frequency;
//uniform vec2 u_textureSize;
float amplitude = 0.05;
float time=0.0;
void main(void)
{
    vec2 pulse = sin(time - frequency * v_texCoord);
    vec2 v_texCoord2 = v_texCoord+ amplitude * pulse;
    myfragcolor=texture2D(texture1,v_texCoord2);
    //myfragcolor=vec4(1,1,1,1);

}
