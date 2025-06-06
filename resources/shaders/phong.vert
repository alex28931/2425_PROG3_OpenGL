#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;
layout (location = 2) in vec3 vert_norm;

uniform mat4 mvp;
uniform mat4 model;

out vec2 vert_uv_out;
out vec3 vert_world_pos_out;
out vec3 vert_world_norm_out;

void main() 
{
    gl_Position = mvp * vec4(vert_pos, 1.f);
    vert_uv_out = vert_uv;

    vert_world_pos_out = ( model * vec4(vert_pos, 1.f) ).xyz;
    //Affect norm by Translation and not proportional scale
    //vert_world_norm_out = ( model * vec4(vert_norm, 1.f) ).xyz;

    vert_world_norm_out = mat3(transpose(inverse(model))) * vert_norm;
}
