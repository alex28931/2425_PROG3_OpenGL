#version 460 core

out vec4 frag_color;

layout (binding = 0) uniform sampler2D storm_tex;

in vec2 vert_uv_out;
in vec3 vert_world_pos_out;
in vec3 vert_world_norm_out;

uniform vec3 point_light_pos;


void main() 
{
    //frag_color = texture(storm_tex, vert_uv_out);

    vec3 pixel_color = texture(storm_tex, vert_uv_out).xyz;

    //Ambient
    float ambient_factor = 0.2f;
    vec3 ambient = pixel_color * ambient_factor;

    //Diffuse
    vec3 light_dir = normalize(point_light_pos - vert_world_pos_out);
    float lambert = max(dot( normalize(vert_world_norm_out), light_dir), 0.f);
    vec3 diffuse = pixel_color * lambert;
    //vec3 diffuse = vec3(1, 0, 0) * lambert;

    //Specular
    //TODO for homework

    //Phong
    vec3 phong = vec3(0, 0, 0);
    phong += ambient;
    phong += diffuse;

    frag_color = vec4(phong, 1);
}
