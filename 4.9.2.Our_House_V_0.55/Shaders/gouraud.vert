#version 330 core

uniform mat4 u_ModelViewMatrix;
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat3 u_NormalMatrix;

uniform vec3 u_light_position;   
uniform vec3 u_view_position;      

uniform vec4 u_material_ambient;
uniform vec4 u_material_diffuse;
uniform vec4 u_material_specular;
uniform float u_material_shininess;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

out vec4 v_color;

void main() {
    vec4 position_view = u_ModelViewMatrix * vec4(a_position, 1.0);
    vec3 pos_view = position_view.xyz;
    vec3 normal_view = normalize(u_NormalMatrix * a_normal);
    vec3 light_dir = normalize(u_light_position - pos_view);
    
    vec3 view_dir = normalize(u_view_position - pos_view);
    vec3 reflect_dir = reflect(-light_dir, normal_view);
    vec3 ambient = u_material_ambient.rgb;
    float diff = max(dot(normal_view, light_dir), 0.0);
    vec3 diffuse = diff * u_material_diffuse.rgb;
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material_shininess);
    vec3 specular = spec * u_material_specular.rgb;
    
    vec3 final_color = ambient + diffuse + specular;
    v_color = vec4(final_color, 1.0);
    
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0);
}