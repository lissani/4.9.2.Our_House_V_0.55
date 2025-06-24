#include "Scene_Definitions.h"
#include "Texture_Manager.h"

void Shader_Simple::prepare_shader() {
	shader_info[0] = { GL_VERTEX_SHADER, "Shaders/simple.vert" };
	shader_info[1] = { GL_FRAGMENT_SHADER, "Shaders/simple.frag" };
	shader_info[2] = { GL_NONE, NULL };

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
	glUseProgram(0);
}

void Shader_Gouraud::prepare_shader() {
    shader_info[0] = { GL_VERTEX_SHADER, "Shaders/gouraud.vert" };
    shader_info[1] = { GL_FRAGMENT_SHADER, "Shaders/gouraud.frag" };
    shader_info[2] = { GL_NONE, NULL };

    h_ShaderProgram = LoadShaders(shader_info);
    glUseProgram(h_ShaderProgram);

    loc_ModelViewMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewMatrix");
    loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
    loc_NormalMatrix = glGetUniformLocation(h_ShaderProgram, "u_NormalMatrix");

    loc_light_position = glGetUniformLocation(h_ShaderProgram, "u_light_position");
    loc_view_position = glGetUniformLocation(h_ShaderProgram, "u_view_position");

    loc_material_ambient = glGetUniformLocation(h_ShaderProgram, "u_material_ambient");
    loc_material_diffuse = glGetUniformLocation(h_ShaderProgram, "u_material_diffuse");
    loc_material_specular = glGetUniformLocation(h_ShaderProgram, "u_material_specular");
    loc_material_shininess = glGetUniformLocation(h_ShaderProgram, "u_material_shininess");

    glUseProgram(0);
}

void Shader_Phong::prepare_shader() {
    shader_info[0] = { GL_VERTEX_SHADER, "Shaders/phong.vert" };
    shader_info[1] = { GL_FRAGMENT_SHADER, "Shaders/phong.frag" };
    shader_info[2] = { GL_NONE, NULL };

    h_ShaderProgram = LoadShaders(shader_info);
    glUseProgram(h_ShaderProgram);

    loc_ModelViewMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewMatrix");
    loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
    loc_NormalMatrix = glGetUniformLocation(h_ShaderProgram, "u_NormalMatrix");

    loc_light_position = glGetUniformLocation(h_ShaderProgram, "u_light_position");
    loc_view_position = glGetUniformLocation(h_ShaderProgram, "u_view_position");

    loc_material_ambient = glGetUniformLocation(h_ShaderProgram, "u_material_ambient");
    loc_material_diffuse = glGetUniformLocation(h_ShaderProgram, "u_material_diffuse");
    loc_material_specular = glGetUniformLocation(h_ShaderProgram, "u_material_specular");
    loc_material_shininess = glGetUniformLocation(h_ShaderProgram, "u_material_shininess");

    glUseProgram(0);
}

void Shader_Gouraud_Texture::prepare_shader() {
    ShaderInfo shader_info[3] = {
        { GL_VERTEX_SHADER, "Shaders/gouraud_texture.vert" },
        { GL_FRAGMENT_SHADER, "Shaders/gouraud_texture.frag" },
        { GL_NONE, NULL }
    };

    h_ShaderProgram = LoadShaders(shader_info);
    if (h_ShaderProgram == 0) {
        fprintf(stderr, "Failed to load Gouraud texture shaders\n");
        exit(EXIT_FAILURE);
    }

    // Uniform 위치들 가져오기
    loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
    loc_ModelViewMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewMatrix");
    loc_NormalMatrix = glGetUniformLocation(h_ShaderProgram, "u_NormalMatrix");
    loc_light_position = glGetUniformLocation(h_ShaderProgram, "u_light_position");
    loc_view_position = glGetUniformLocation(h_ShaderProgram, "u_view_position");

    loc_material_ambient = glGetUniformLocation(h_ShaderProgram, "u_material.ambient");
    loc_material_diffuse = glGetUniformLocation(h_ShaderProgram, "u_material.diffuse");
    loc_material_specular = glGetUniformLocation(h_ShaderProgram, "u_material.specular");
    loc_material_shininess = glGetUniformLocation(h_ShaderProgram, "u_material.shininess");

    // 텍스처 관련 uniform 위치
    loc_texture_sampler = glGetUniformLocation(h_ShaderProgram, "u_texture_sampler");
    loc_use_texture = glGetUniformLocation(h_ShaderProgram, "u_use_texture");
}

void Shader_Phong_Texture::prepare_shader() {
    ShaderInfo shader_info[3] = {
        { GL_VERTEX_SHADER, "Shaders/phong_texture.vert" },
        { GL_FRAGMENT_SHADER, "Shaders/phong_texture.frag" },
        { GL_NONE, NULL }
    };

    h_ShaderProgram = LoadShaders(shader_info);
    if (h_ShaderProgram == 0) {
        fprintf(stderr, "Failed to load Phong texture shaders\n");
        exit(EXIT_FAILURE);
    }

    // Uniform 위치들 가져오기 (Gouraud와 동일)
    loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
    loc_ModelViewMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewMatrix");
    loc_NormalMatrix = glGetUniformLocation(h_ShaderProgram, "u_NormalMatrix");
    loc_light_position = glGetUniformLocation(h_ShaderProgram, "u_light_position");
    loc_view_position = glGetUniformLocation(h_ShaderProgram, "u_view_position");

    loc_material_ambient = glGetUniformLocation(h_ShaderProgram, "u_material.ambient");
    loc_material_diffuse = glGetUniformLocation(h_ShaderProgram, "u_material.diffuse");
    loc_material_specular = glGetUniformLocation(h_ShaderProgram, "u_material.specular");
    loc_material_shininess = glGetUniformLocation(h_ShaderProgram, "u_material.shininess");

    loc_texture_sampler = glGetUniformLocation(h_ShaderProgram, "u_texture_sampler");
    loc_use_texture = glGetUniformLocation(h_ShaderProgram, "u_use_texture");
}