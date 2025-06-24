#version 330 core

in vec3 v_color;
in vec2 v_tex_coord;

uniform sampler2D u_texture_sampler;
uniform bool u_use_texture;

out vec4 final_color;

void main() {
    if (u_use_texture) {
        vec4 tex_color = texture(u_texture_sampler, v_tex_coord);
        final_color = vec4(v_color * tex_color.rgb, tex_color.a);
    } else {
        final_color = vec4(v_color, 1.0);
    }
}