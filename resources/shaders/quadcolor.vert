#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_col;

out vec4 vert_col_out;

uniform float scale;

void main() 
{
    //gl_Position = vec4(vert_pos, 1.f);
    
    vec3 vert_pos_scaled = vert_pos * scale;
    gl_Position = vec4(vert_pos_scaled, 1.f);

    vert_col_out = vec4(vert_col, 1.f);
}
