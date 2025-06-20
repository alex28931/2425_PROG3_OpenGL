#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec4 vert_color;

out vec4 frag_color;

void main() 
{
    gl_Position = vec4(vert_pos, 1.0);
    frag_color = vert_color;
}