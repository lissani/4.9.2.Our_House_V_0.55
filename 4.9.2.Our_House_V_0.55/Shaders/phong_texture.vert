#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat3 u_NormalMatrix;

out vec3 v_position_view;
out vec3 v_normal_view;
out vec2 v_tex_coord;

void main() {
    v_position_view = (u_ModelViewMatrix * vec4(a_position, 1.0)).xyz;
    v_normal_view = normalize(u_NormalMatrix * a_normal);
    v_tex_coord = a_tex_coord;
    
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);
}