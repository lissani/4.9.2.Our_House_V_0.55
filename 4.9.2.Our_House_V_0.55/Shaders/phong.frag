#version 330 core

in vec3 v_pos_eye;
in vec3 v_normal_eye;

uniform vec3 u_light_position;
uniform vec3 u_view_position;

uniform vec4 u_material_ambient;
uniform vec4 u_material_diffuse;
uniform vec4 u_material_specular;
uniform float u_material_shininess;

out vec4 frag_color;

void main() {
    vec3 normal = normalize(v_normal_eye);
    vec3 light_dir = normalize(u_light_position - v_pos_eye);
    vec3 view_dir = normalize(u_view_position - v_pos_eye);
    vec3 reflect_dir = reflect(-light_dir, normal);

    float diff = max(dot(normal, light_dir), 0.0);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material_shininess);

    vec3 ambient = u_material_ambient.rgb;
    vec3 diffuse = u_material_diffuse.rgb * diff;
    vec3 specular = u_material_specular.rgb * spec;

    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0);
}
