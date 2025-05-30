#version 460 core

out vec4 frag_color;

in vec2 vert_uv_out;

layout (binding = 0) uniform sampler2D box_tex;

void main() 
{
    vec4 box_texel = texture(box_tex, vert_uv_out);
    frag_color = box_texel;
}
