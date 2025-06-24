#version 400

// 버텍스 쉐이더에서 받은 입력
in vec3 v_position_view;
in vec3 v_normal_view;
in vec2 v_texcoord_out;

// 다중 조명
struct Light {
    vec3 position;
    vec3 color;
    bool enabled;
};

uniform Light u_world_light;
uniform Light u_view_light;
uniform Light u_model_light;

// 머티리얼
uniform vec4 u_material_ambient;
uniform vec4 u_material_diffuse;
uniform vec4 u_material_specular;
uniform float u_material_shininess;

// 텍스처
uniform sampler2D u_texture_sampler;
uniform bool u_use_texture;

// 출력
out vec4 final_color;

vec3 calculateLighting(vec3 position_view, vec3 normal_view, Light light) {
    if (!light.enabled) return vec3(0.0);
    
    vec3 light_dir = normalize(light.position - position_view);
    vec3 view_dir = normalize(-position_view);
    vec3 reflect_dir = reflect(-light_dir, normal_view);
    
    // Ambient
    vec3 ambient = u_material_ambient.rgb * light.color;
    
    // Diffuse
    float diff = max(dot(normal_view, light_dir), 0.0);
    vec3 diffuse = diff * u_material_diffuse.rgb * light.color;
    
    // Specular
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material_shininess);
    vec3 specular = spec * u_material_specular.rgb * light.color;
    
    return ambient + diffuse + specular;
}

void main(void) {
    vec3 normal = normalize(v_normal_view);
    
    // 각 광원별 조명 계산
    vec3 total_lighting = vec3(0.0);
    
    // 세상좌표계 광원
    total_lighting += calculateLighting(v_position_view, normal, u_world_light);
    
    // 눈좌표계 광원  
    total_lighting += calculateLighting(v_position_view, normal, u_view_light);
    
    // 모델좌표계 광원
    total_lighting += calculateLighting(v_position_view, normal, u_model_light);
    
    // 텍스처 적용
    vec4 texture_color = vec4(1.0); // 기본값: 흰색
    if (u_use_texture) {
        texture_color = texture(u_texture_sampler, v_texcoord_out);
    }
    
    // 최종 색상 = 조명 계산 결과 * 텍스처 색상
    vec3 lit_color = clamp(total_lighting, 0.0, 1.0);
    final_color = vec4(lit_color * texture_color.rgb, u_material_diffuse.a * texture_color.a);
}