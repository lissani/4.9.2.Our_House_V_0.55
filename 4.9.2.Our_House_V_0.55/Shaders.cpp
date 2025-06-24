#include "Scene_Definitions.h"

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
