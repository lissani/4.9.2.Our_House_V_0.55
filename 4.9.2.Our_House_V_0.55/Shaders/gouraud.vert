#version 400

// 입력 속성
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;

// 유니폼 변수들
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat4 u_ModelViewMatrix;
uniform mat3 u_NormalMatrix;

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

// 출력 (프래그먼트 쉐이더로)
out vec4 v_color;
out vec2 v_texcoord_out;

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
    // 위치와 법선을 눈좌표계로 변환
    vec3 position_view = (u_ModelViewMatrix * vec4(v_position, 1.0)).xyz;
    vec3 normal_view = normalize(u_NormalMatrix * v_normal);
    
    // 각 광원별 조명 계산
    vec3 total_lighting = vec3(0.0);
    
    // 세상좌표계 광원
    total_lighting += calculateLighting(position_view, normal_view, u_world_light);
    
    // 눈좌표계 광원
    total_lighting += calculateLighting(position_view, normal_view, u_view_light);
    
    // 모델좌표계 광원
    total_lighting += calculateLighting(position_view, normal_view, u_model_light);
    
    // 최종 색상
    v_color = vec4(clamp(total_lighting, 0.0, 1.0), u_material_diffuse.a);
    v_texcoord_out = v_texcoord;
    
    gl_Position = u_ModelViewProjectionMatrix * vec4(v_position, 1.0);
}