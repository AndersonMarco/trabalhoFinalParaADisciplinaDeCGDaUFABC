#version 130
in vec4 vPosition;
in vec3 normalVertice;
out vec4 color_out;
uniform mat4 mModelView;
uniform mat4 mCamera;
uniform mat4 mProjection;

void main(void)
{

    gl_Position=mProjection*mCamera*mModelView*vPosition;



    color_out = vec4(abs(normalVertice),1);


}

