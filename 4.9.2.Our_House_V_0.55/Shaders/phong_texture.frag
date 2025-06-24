#version 330 core

in vec3 v_position_view;
in vec3 v_normal_view;
in vec2 v_tex_coord;

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

uniform sampler2D u_texture_sampler;
uniform bool u_use_texture;

out vec4 final_color;

void main() {
    vec3 normal = normalize(v_normal_view);
    vec3 light_dir = normalize(u_light_position - v_position_view);
    vec3 view_dir = normalize(u_view_position - v_position_view);
    vec3 reflect_dir = reflect(-light_dir, normal);
    
    // Ambient
    vec3 ambient = u_material.ambient.rgb;
    
    // Diffuse
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * u_material.diffuse.rgb;
    
    // Specular
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
    vec3 specular = spec * u_material.specular.rgb;
    
    vec3 lighting = ambient + diffuse + specular;
    
    if (u_use_texture) {
        vec4 tex_color = texture(u_texture_sampler, v_tex_coord);
        final_color = vec4(lighting * tex_color.rgb, tex_color.a);
    } else {
        final_color = vec4(lighting, 1.0);
    }
}