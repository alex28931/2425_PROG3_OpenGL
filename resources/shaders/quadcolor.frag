#version 460 core

out vec4 frag_color;

in vec4 vert_col_out;

uniform vec4 base_color;

void main() 
{
    //frag_color = vec4(1.f, 0.f, 0.f, 1.f);
    //frag_color = vert_col_out;
    frag_color = vert_col_out;
}
