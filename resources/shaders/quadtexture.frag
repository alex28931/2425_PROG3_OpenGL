#version 460 core

out vec4 frag_color;

in vec2 vert_uv_out;

//uniform sampler2D smile_tex;
layout (binding = 0) uniform sampler2D smile_tex;
layout (binding = 1) uniform sampler2D box_tex;

void main() 
{
    //frag_color = texture(smile_tex, vert_uv_out);
    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 box_texel = texture(box_tex, vert_uv_out);
    
    //vec3 full_transp = vec4(0, 0, 0, 0); 
    //vec3 mixed_rgb = mix(smile_texel.rgb, box_texel.rgb, 0.5f);
    //frag_color = vec4(mixed_rgb, 1.f);

    vec4 color;
    if (smile_texel.a > 0.f) {
        color = mix(smile_texel, box_texel, 0.5f);
    } else {
        color = box_texel;
    }

    //float factor = (1.f - (smile_texel.a * (1.f - 0.5f)));
    //color = mix(smile_texel, box_texel, factor);

    //alpha = 0  => is_visible = 0
    //alpha > 0  => is_visible = 1
    //float is_visible = step(0.f, smile_texel.a);

    frag_color = color;
}
