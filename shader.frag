#version 130

in vec4 color_out;
out vec4 myfragcolor;
void main(void)
{
    myfragcolor=color_out;
}

