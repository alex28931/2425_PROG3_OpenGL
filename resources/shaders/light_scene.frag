#version 460 core

out vec4 frag_color;

layout (binding = 0) uniform sampler2D storm_tex;

in vec2 vert_uv_out;
in vec3 vert_world_pos_out;
in vec3 vert_world_norm_out;

uniform vec3 point_light_pos;
uniform vec3 camera_pos;


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
    vec3 dir_to_eye = normalize(camera_pos - vert_world_pos_out);
    vec3 reflected =reflect((-light_dir), normalize(vert_world_norm_out));
    float beta = max(dot(dir_to_eye, reflected), 0.0);
    float specular_strength = 0.5;
    float shininess = 50.0;
    vec3 specular_color = vec3(1.0);
    vec3 specular = specular_color * specular_strength * pow(beta, shininess);

    //Phong
    vec3 phong = vec3(0, 0, 0);
    phong += ambient;
    phong += diffuse;
    phong += specular;

    frag_color = vec4(phong, 1);
}
