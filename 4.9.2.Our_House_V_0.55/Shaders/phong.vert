#version 400

// 입력 속성
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;

// 유니폼 변수들
uniform mat4 u_ModelViewProjectionMatrix;
uniform mat4 u_ModelViewMatrix;
uniform mat3 u_NormalMatrix;

// 출력 (프래그먼트 쉐이더로)
out vec3 v_position_view;
out vec3 v_normal_view;
out vec2 v_texcoord_out;

void main(void) {
    // 위치와 법선을 눈좌표계로 변환하여 프래그먼트 쉐이더로 전달
    v_position_view = (u_ModelViewMatrix * vec4(v_position, 1.0)).xyz;
    v_normal_view = normalize(u_NormalMatrix * v_normal);
    v_texcoord_out = v_texcoord;
    
    gl_Position = u_ModelViewProjectionMatrix * vec4(v_position, 1.0);
}