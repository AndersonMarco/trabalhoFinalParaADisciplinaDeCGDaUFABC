#version 130
in vec4 vPosition;


in vec2 a_texCoord;
out vec2 v_texCoord;
uniform mat4 mModelView;
uniform mat4 mProjection;

void main(void)
{
    vec4 eyePosition = mModelView * vPosition;
    gl_Position=mProjection*eyePosition;

    v_texCoord=a_texCoord;
}
