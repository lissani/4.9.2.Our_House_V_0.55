#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat3 u_NormalMatrix;

out vec3 v_pos_eye;
out vec3 v_normal_eye;

void main() {
    v_pos_eye = vec3(u_ModelViewMatrix * vec4(a_position, 1.0));
    v_normal_eye = normalize(u_NormalMatrix * a_normal);
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);
}
