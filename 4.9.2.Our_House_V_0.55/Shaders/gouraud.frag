#version 400

// 버텍스 쉐이더에서 받은 입력
in vec4 v_color;
in vec2 v_texcoord_out;

// 출력
out vec4 final_color;

void main(void) {
    final_color = v_color;
}