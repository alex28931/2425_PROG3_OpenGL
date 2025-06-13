#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

out vec2 vert_uv_out;

vec3 perspective(vec3 pos) 
{
    float aspect_ratio = 800.f / 600.f;
    float vertical_fov = 60.f;
    float fov = tan(radians(vertical_fov * 0.5f));

    vec3 result;
    result.x = pos.x / (fov * aspect_ratio);
    result.y = pos.y / fov;
    result.z = pos.z;
    return result;
}

void main() 
{
    vec3 fixed_vert = perspective(vert_pos);
    gl_Position = vec4(fixed_vert, 1.f);
    vert_uv_out = vert_uv;
}
