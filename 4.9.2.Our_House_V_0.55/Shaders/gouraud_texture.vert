#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat3 u_NormalMatrix;
uniform vec3 u_light_position;
uniform vec3 u_view_position;

// 머티리얼 구조체
struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
uniform Material u_material;

out vec3 v_color;
out vec2 v_tex_coord;

void main() {
    vec4 position_view = u_ModelViewMatrix * vec4(a_position, 1.0);
    vec3 normal_view = normalize(u_NormalMatrix * a_normal);
    
    // 라이트와 뷰 방향 계산
    vec3 light_dir = normalize(u_light_position - position_view.xyz);
    vec3 view_dir = normalize(u_view_position - position_view.xyz);
    vec3 reflect_dir = reflect(-light_dir, normal_view);
    
    // Ambient
    vec3 ambient = u_material.ambient.rgb;
    
    // Diffuse
    float diff = max(dot(normal_view, light_dir), 0.0);
    vec3 diffuse = diff * u_material.diffuse.rgb;
    
    // Specular
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
    vec3 specular = spec * u_material.specular.rgb;
    
    v_color = ambient + diffuse + specular;
    v_tex_coord = a_tex_coord;
    
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);
}