#version 460 core

out vec4 frag_color;

layout (binding = 0) uniform sampler2D diffuse_tex;
layout (binding = 1) uniform sampler2D normal_tex;
layout (binding = 2) uniform sampler2D position_tex;

in vec2 vert_uv_out;

const int MAX_NUM_LIGHTS = 1000;
uniform vec3 point_light_pos[MAX_NUM_LIGHTS];
uniform float light_count;

vec3 compute_phong(int light_index) {
    
    vec3 pixel_color = texture(diffuse_tex, vert_uv_out).xyz;

    //Ambient
    float ambient_factor = 0.2f;
    vec3 ambient = pixel_color * ambient_factor;

    //Diffuse
    vec3 vert_world_pos_out = texture(position_tex, vert_uv_out).xyz;
    vec3 vert_world_norm_out = texture(normal_tex, vert_uv_out).xyz;

    vec3 light_dir = normalize(point_light_pos[light_index] - vert_world_pos_out);
    float lambert = max(dot( vert_world_norm_out, light_dir), 0.f);
    vec3 diffuse = pixel_color * lambert;

    //Specular
    //TODO for homework

    //Phong
    vec3 phong = vec3(0, 0, 0);
    phong += ambient;
    phong += diffuse;

    return phong;
}

void main() {

    vec3 contrib = vec3(0);
    for(int i = 0; i < light_count; ++i) {
        contrib += compute_phong(i);
    }

    frag_color = vec4(contrib, 1.f);
}