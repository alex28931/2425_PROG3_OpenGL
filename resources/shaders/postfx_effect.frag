#version 460 core

out vec4 frag_color;

layout (binding = 0) uniform sampler2D scene_texture;

in vec2 vert_uv_out;

uniform float time;

vec4 blur() {
    vec2 texture_size = textureSize(scene_texture, 0);
    float uv_w = 1.f / texture_size.x;
    float uv_h = 1.f / texture_size.y;

    vec4 contrib = vec4(0);

    //taking 7x7 pixel matrix
    for(int i=-3; i <= 3; ++i) {
        
        float xoff = uv_w * i;
        for(int j=-3; j <= 3; ++j) {
            float yoff = uv_h * j;
            vec2 uv_corr = vec2(vert_uv_out.x + xoff, vert_uv_out.y + yoff);
            contrib += texture(scene_texture, uv_corr);
        }    
    }

    vec4 result = contrib / 49;
    return result;
}

vec4 wave() {
    //y = A sin(B(x + C)) + D

    float A = 1.f / 70.f;
    float B = 20.f;
    //float C = 1.f / 40.f;
    float C = time / 40.f;
    float D = 0.f;

    vec2 uv_curr = vert_uv_out;
    uv_curr.x += A * sin(B * (uv_curr.y + C)) + D;
    return texture(scene_texture, uv_curr);
}

vec4 quake() {
    vec2 uv_curr = vert_uv_out; 
    float strength = 0.01;

    uv_curr.x += cos(time * 30) * strength;
    uv_curr.y += cos(time * 45) * strength;

    return texture(scene_texture, uv_curr);
}


void main() 
{
    // As-is
    //frag_color = texture(scene_texture, vert_uv_out);

    /* GRAY SCALE 
    vec4 color = texture(scene_texture, vert_uv_out);
    float avg = (color.r + color.g + color.b) / 3.0;
    frag_color = vec4(avg, avg, avg, 1.f);
    */

    /* NEGATIVE
    vec4 color = texture(scene_texture, vert_uv_out);
    frag_color = 1.f - color;
    */

    /* BLACKBAND with pixel coord
    if (gl_FragCoord.y < 30 || gl_FragCoord.y > 570) {
        frag_color = vec4(0);
    } else {
        frag_color = texture(scene_texture, vert_uv_out);
    }
    */

    /* BLACKBAND with uv coord
    if (vert_uv_out.y < 0.1 || vert_uv_out.y > 0.9) {
        frag_color = vec4(0);
    } else {
        frag_color = texture(scene_texture, vert_uv_out);
    }
    */

    //frag_color = blur();
    
    //frag_color = wave();
    
    frag_color = quake();
}
