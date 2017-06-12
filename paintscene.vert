#version 130
in vec2 a_position;
uniform vec2 u_resolution;
in vec2 a_texCoord;


out vec2 v_texCoord;

void main() {
    // convert the rectangle from pixels to 0.0 to 1.0
    vec2 zeroToOne = a_position/u_resolution;

    gl_Position = vec4((zeroToOne* vec2(2, -2))-vec2(1,-1), 0, 1);

    // pass the texCoord to the fragment shader
    // The GPU will interpolate this value between points.
    v_texCoord = a_texCoord;
}
