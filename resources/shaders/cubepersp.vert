#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

out vec2 vert_uv_out;

vec3 perspective(vec3 pos) 
{
    float aspect_ratio = 800.f / 600.f;
    float vertical_fov = 60.f;
    float fov = tan(radians(vertical_fov * 0.5f));

    float near = 0.01f;
    float far  = 100.f;
    float range = far - near;

    vec3 result;
    result.x = pos.x / -pos.z;
    result.y = pos.y / -pos.z;

    //RHS -> LHS
    result.z = ( (pos.z - near) / range ) * 2.f + 1.f;
    result.z *= -1;

    result.x /= (fov * aspect_ratio);
    result.y /= fov;
    return result;
}

vec3 rotate_on_y(vec3 pos, float degrees) 
{
    float rads = radians(degrees);
    vec3 result;
    result.x = cos(rads) * pos.x - sin(rads) * pos.z;
    result.y = pos.y;
    result.z = sin(rads) * pos.x + cos(rads) * pos.z;
    return result;
}

uniform float angle;

void main() 
{
    vec3 fixed_vert = vert_pos;

    fixed_vert = rotate_on_y(fixed_vert, angle);
    fixed_vert.z -= 4.f;  //translate;
    fixed_vert = perspective(fixed_vert);
    gl_Position = vec4(fixed_vert, 1.f);
    vert_uv_out = vert_uv;
}
