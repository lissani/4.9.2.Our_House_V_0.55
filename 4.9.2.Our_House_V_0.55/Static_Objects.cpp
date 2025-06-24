#define _CRT_SECURE_NO_WARNINGS

#include "Scene_Definitions.h"

void set_lighting_uniforms_gouraud(Shader_Gouraud* shader, glm::mat4& ViewMatrix, Scene& scene) {
	// 세상좌표계 광원 (World Light)
	glm::vec3 world_light_view = glm::vec3(ViewMatrix * glm::vec4(scene.world_light.position, 1.0f));
	glUniform3fv(shader->loc_world_light_position, 1, &world_light_view[0]);
	glUniform3fv(shader->loc_world_light_color, 1, &scene.world_light.color[0]);
	glUniform1i(shader->loc_world_light_enabled, scene.world_light_enabled ? 1 : 0);

	// 눈좌표계 광원 (View Light) - 이미 눈좌표계에 있으므로 변환 불필요
	glm::vec3 view_light_position_eye = glm::vec3(0.0f, 0.0f, -50.0f); // 카메라 앞쪽
	glUniform3fv(shader->loc_view_light_position, 1, &view_light_position_eye[0]);
	glUniform3fv(shader->loc_view_light_color, 1, &scene.view_light.color[0]);
	glUniform1i(shader->loc_view_light_enabled, scene.view_light_enabled ? 1 : 0);

	// 모델좌표계 광원 (정적 객체는 사용 안함)
	glUniform3fv(shader->loc_model_light_position, 1, &scene.model_light.position[0]);
	glUniform3fv(shader->loc_model_light_color, 1, &scene.model_light.color[0]);
	glUniform1i(shader->loc_model_light_enabled, 0); // 정적 객체는 모델 광원 없음
}

void set_lighting_uniforms_phong(Shader_Phong* shader, glm::mat4& ViewMatrix, Scene& scene) {
	// 세상좌표계 광원 (World Light)
	glm::vec3 world_light_view = glm::vec3(ViewMatrix * glm::vec4(scene.world_light.position, 1.0f));
	glUniform3fv(shader->loc_world_light_position, 1, &world_light_view[0]);
	glUniform3fv(shader->loc_world_light_color, 1, &scene.world_light.color[0]);
	glUniform1i(shader->loc_world_light_enabled, scene.world_light_enabled ? 1 : 0);

	// 눈좌표계 광원 (View Light)
	glm::vec3 view_light_position_eye = glm::vec3(0.0f, 0.0f, -50.0f); // 카메라 앞쪽
	glUniform3fv(shader->loc_view_light_position, 1, &view_light_position_eye[0]);
	glUniform3fv(shader->loc_view_light_color, 1, &scene.view_light.color[0]);
	glUniform1i(shader->loc_view_light_enabled, scene.view_light_enabled ? 1 : 0);

	// 모델좌표계 광원
	glUniform3fv(shader->loc_model_light_position, 1, &scene.model_light.position[0]);
	glUniform3fv(shader->loc_model_light_color, 1, &scene.model_light.color[0]);
	glUniform1i(shader->loc_model_light_enabled, 0); // 정적 객체는 모델 광원 없음
}

void set_lighting_uniforms_gouraud_texture(Shader_Gouraud_Texture* shader, glm::mat4& ViewMatrix, Scene& scene) {
	// 세상좌표계 광원 (World Light)
	glm::vec3 world_light_view = glm::vec3(ViewMatrix * glm::vec4(scene.world_light.position, 1.0f));
	glUniform3fv(shader->loc_world_light_position, 1, &world_light_view[0]);
	glUniform3fv(shader->loc_world_light_color, 1, &scene.world_light.color[0]);
	glUniform1i(shader->loc_world_light_enabled, scene.world_light_enabled ? 1 : 0);

	// 눈좌표계 광원 (View Light)
	glm::vec3 view_light_position_eye = glm::vec3(0.0f, 0.0f, -50.0f); // 카메라 앞쪽
	glUniform3fv(shader->loc_view_light_position, 1, &view_light_position_eye[0]);
	glUniform3fv(shader->loc_view_light_color, 1, &scene.view_light.color[0]);
	glUniform1i(shader->loc_view_light_enabled, scene.view_light_enabled ? 1 : 0);

	// 모델좌표계 광원
	glUniform3fv(shader->loc_model_light_position, 1, &scene.model_light.position[0]);
	glUniform3fv(shader->loc_model_light_color, 1, &scene.model_light.color[0]);
	glUniform1i(shader->loc_model_light_enabled, 0); // 정적 객체는 모델 광원 없음
}

void set_lighting_uniforms_phong_texture(Shader_Phong_Texture* shader, glm::mat4& ViewMatrix, Scene& scene) {
	// 세상좌표계 광원 (World Light)
	glm::vec3 world_light_view = glm::vec3(ViewMatrix * glm::vec4(scene.world_light.position, 1.0f));
	glUniform3fv(shader->loc_world_light_position, 1, &world_light_view[0]);
	glUniform3fv(shader->loc_world_light_color, 1, &scene.world_light.color[0]);
	glUniform1i(shader->loc_world_light_enabled, scene.world_light_enabled ? 1 : 0);

	// 눈좌표계 광원 (View Light)
	glm::vec3 view_light_position_eye = glm::vec3(0.0f, 0.0f, -50.0f); // 카메라 앞쪽
	glUniform3fv(shader->loc_view_light_position, 1, &view_light_position_eye[0]);
	glUniform3fv(shader->loc_view_light_color, 1, &scene.view_light.color[0]);
	glUniform1i(shader->loc_view_light_enabled, scene.view_light_enabled ? 1 : 0);

	// 모델좌표계 광원
	glUniform3fv(shader->loc_model_light_position, 1, &scene.model_light.position[0]);
	glUniform3fv(shader->loc_model_light_color, 1, &scene.model_light.color[0]);
	glUniform1i(shader->loc_model_light_enabled, 0); // 정적 객체는 모델 광원 없음
}

void Static_Object::read_geometry(int bytes_per_primitive) {
	FILE* fp;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Error: cannot open the object file %s ...\n", filename);
		exit(EXIT_FAILURE);
	}
	fread(&n_triangles, sizeof(int), 1, fp);
	vertices = (float*)malloc(n_triangles * bytes_per_primitive);
	if (vertices == NULL) {
		fprintf(stderr, "Error: cannot allocate memory for the geometry file %s ...\n", filename);
		exit(EXIT_FAILURE);
	}
	fread(vertices, bytes_per_primitive, n_triangles, fp);
	fclose(fp);
}

void Static_Object::prepare_geom_of_static_object() {
	int i, n_bytes_per_vertex, n_bytes_per_triangle;
	char filename[512];

	n_bytes_per_vertex = n_fields * sizeof(float);
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	read_geometry(n_bytes_per_triangle);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n_triangles * n_bytes_per_triangle, vertices, GL_STATIC_DRAW);

	free(vertices);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, (GLvoid*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

	if (n_fields == 8) {
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, (GLvoid*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Building::define_object() {
	Material* cur_material;
	strcpy(filename, "Data/Building1_vnt.geom");
	n_fields = 8;
	front_face_mode = GL_CCW;
	prepare_geom_of_static_object();
	flag_valid = true;

	instances.emplace_back();
	instances.back().ModelMatrix = glm::mat4(1.0f);

	cur_material = &(instances.back().material);
	cur_material->emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cur_material->ambient = glm::vec4(0.135f, 0.2225f, 0.1575f, 1.0f);
	cur_material->diffuse = glm::vec4(0.54f, 0.89f, 0.63f, 1.0f);
	cur_material->specular = glm::vec4(0.316228f, 0.316228f, 0.316228f, 1.0f);
	cur_material->exponent = 128.0f * 0.1f;
}

void Ant::define_object() {
	glm::mat4* cur_MM;
	Material* cur_material;
	strcpy(filename, "Data/static_objects/ant_vnt.geom");
	n_fields = 8;
	front_face_mode = GL_CCW;
	prepare_geom_of_static_object();
	flag_valid = true;

	instances.emplace_back();
	cur_MM = &(instances.back().ModelMatrix);
	*cur_MM = glm::translate(glm::mat4(1.0f), glm::vec3(192.0f, 145.5f, 0.0));
	*cur_MM = glm::scale(*cur_MM, glm::vec3(13.5f * 0.3f, 13.5f * 0.3f, 13.5f * 0.3f));
	*cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	cur_material = &(instances.back().material);
	cur_material->emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cur_material->ambient = glm::vec4(0.24725f, 0.1995f, 0.0745f, 1.0f);
	cur_material->diffuse = glm::vec4(0.75164f, 0.60648f, 0.22648f, 1.0f);
	cur_material->specular = glm::vec4(0.628281f, 0.555802f, 0.366065f, 1.0f);
	cur_material->exponent = 128.0f * 0.4f;
}

void Wood_Tower::define_object() {
	glm::mat4* cur_MM;
	Material* cur_material;
	strcpy(filename, "Data/static_objects/woodTower_vnt.geom");
	n_fields = 8;
	front_face_mode = GL_CCW;
	prepare_geom_of_static_object();
	flag_valid = true;

	force_texture_shader = true;

	instances.emplace_back();
	cur_MM = &(instances.back().ModelMatrix);
	*cur_MM = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, 134.0f, 0.0f));
	*cur_MM = glm::scale(*cur_MM, glm::vec3(13.5f * 0.4f, 13.5f * 0.4f, 13.5f * 0.4f));
	*cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	cur_material = &(instances.back().material);
	cur_material->emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cur_material->ambient = glm::vec4(0.1f, 0.3f, 0.1f, 1.0f);
	cur_material->diffuse = glm::vec4(0.4f, 0.6f, 0.3f, 1.0f);
	cur_material->specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	cur_material->exponent = 15.0f;

	GLuint texture_id = texture_manager.load_texture("Data/static_objects/Wood_Tower_Col.jpg");

	for (int i = 0; i < instances.size(); i++) {
		instances[i].texture_id = texture_id;
		instances[i].use_texture = true;
	}
}

void Cat::define_object() {
	glm::mat4* cur_MM;
	Material* cur_material;
	strcpy(filename, "Data/static_objects/cat_vnt.geom");
	n_fields = 8;
	front_face_mode = GL_CCW;
	prepare_geom_of_static_object();
	flag_valid = true;

	instances.emplace_back();
	cur_MM = &(instances.back().ModelMatrix);
	*cur_MM = glm::translate(glm::mat4(1.0f), glm::vec3(216.0f, 23.0f, 0.0f));
	*cur_MM = glm::scale(*cur_MM, glm::vec3(13.5f * 0.9f, 13.5f * 0.9f, 13.5f * 0.9f));
	*cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, -1.0f));
	cur_material = &(instances.back().material);
	*cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	cur_material->emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cur_material->ambient = glm::vec4(0.05f, 0.05f, 0.0f, 1.0f);
	cur_material->diffuse = glm::vec4(0.5f, 0.5f, 0.4f, 1.0f);
	cur_material->specular = glm::vec4(0.7f, 0.7f, 0.04f, 1.0f);
	cur_material->exponent = 128.0f * 0.078125f;
}

void Iron_Man::define_object() {
	glm::mat4* cur_MM;
	Material* cur_material;
	strcpy(filename, "Data/static_objects/ironman_vnt.geom");
	n_fields = 8;
	front_face_mode = GL_CCW;
	prepare_geom_of_static_object();
	flag_valid = true;

	instances.emplace_back();
	cur_MM = &(instances.back().ModelMatrix);
	*cur_MM = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 43.0f, 0.0f));
	*cur_MM = glm::scale(*cur_MM, glm::vec3(13.5f * 0.4f, 13.5f * 0.4f, 13.5f * 0.4f));
	*cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	cur_material = &(instances.back().material);
	cur_material->emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cur_material->ambient = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
	cur_material->diffuse = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
	cur_material->specular = glm::vec4(0.774597f, 0.774597f, 0.774597f, 1.0f);
	cur_material->exponent = 128.0f * 0.6f;
}

void Bike::define_object() {
	glm::mat4* cur_MM;
	Material* cur_material;
	strcpy(filename, "Data/static_objects/bike_vnt.geom");
	n_fields = 8;
	front_face_mode = GL_CCW;
	prepare_geom_of_static_object();
	flag_valid = true;

	instances.emplace_back();
	cur_MM = &(instances.back().ModelMatrix);
	*cur_MM = glm::translate(glm::mat4(1.0f), glm::vec3(90.0f, 137.0f, 0.0f));
	*cur_MM = glm::scale(*cur_MM, glm::vec3(10.0f, 10.0f, 10.0f));
	*cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	*cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	cur_material = &(instances.back().material);
	cur_material->emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cur_material->ambient = glm::vec4(0.329412f, 0.223529f, 0.027451f, 1.0f);
	cur_material->diffuse = glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f);
	cur_material->specular = glm::vec4(0.992157f, 0.941176f, 0.807843f, 1.0f);
	cur_material->exponent = 0.21794872f * 0.6f;
}

void Static_Object::draw_object(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
	std::vector<std::reference_wrapper<Shader>>& shader_list, Scene& scene) {
	glm::mat4 ModelViewProjectionMatrix;
	glFrontFace(front_face_mode);

	for (int i = 0; i < instances.size(); i++) {
		ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * instances[i].ModelMatrix;

		SHADER_ID actual_shader = shader_kind;

		// Wood Tower는 항상 Phong Texture 쉐이더 사용
		if (force_texture_shader && instances[i].use_texture && instances[i].texture_id != 0) {
			actual_shader = SHADER_PHONG_TEXTURE;
		}

		switch (actual_shader) {
		case SHADER_SIMPLE: {
			Shader_Simple* shader_simple_ptr = static_cast<Shader_Simple*>(&shader_list[shader_ID_mapper[actual_shader]].get());
			glUseProgram(shader_simple_ptr->h_ShaderProgram);
			glUniformMatrix4fv(shader_simple_ptr->loc_ModelViewProjectionMatrix, 1, GL_FALSE,
				&ModelViewProjectionMatrix[0][0]);
			glUniform3f(shader_simple_ptr->loc_primitive_color, instances[i].material.diffuse.r,
				instances[i].material.diffuse.g, instances[i].material.diffuse.b);
			break;
		}
		case SHADER_GOURAUD: {
			auto* shader = static_cast<Shader_Gouraud*>(&shader_list[shader_ID_mapper[actual_shader]].get());
			glUseProgram(shader->h_ShaderProgram);

			glm::mat4 ModelMatrix = instances[i].ModelMatrix;
			glm::mat4 MV = ViewMatrix * ModelMatrix;
			glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));
			glm::mat4 MVP = ProjectionMatrix * MV;

			glUniformMatrix4fv(shader->loc_ModelViewMatrix, 1, GL_FALSE, &MV[0][0]);
			glUniformMatrix4fv(shader->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix3fv(shader->loc_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);

			// 다중 조명 설정
			set_lighting_uniforms_gouraud(shader, ViewMatrix, scene);

			Material& m = instances[i].material;
			glUniform4fv(shader->loc_material_ambient, 1, &m.ambient[0]);
			glUniform4fv(shader->loc_material_diffuse, 1, &m.diffuse[0]);
			glUniform4fv(shader->loc_material_specular, 1, &m.specular[0]);
			glUniform1f(shader->loc_material_shininess, m.exponent);
			break;
		}
		case SHADER_PHONG: {
			auto* shader = static_cast<Shader_Phong*>(&shader_list[shader_ID_mapper[actual_shader]].get());
			glUseProgram(shader->h_ShaderProgram);

			glm::mat4 ModelMatrix = instances[i].ModelMatrix;
			glm::mat4 MV = ViewMatrix * ModelMatrix;
			glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));
			glm::mat4 MVP = ProjectionMatrix * MV;

			glUniformMatrix4fv(shader->loc_ModelViewMatrix, 1, GL_FALSE, &MV[0][0]);
			glUniformMatrix4fv(shader->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix3fv(shader->loc_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);

			// 다중 조명 설정
			set_lighting_uniforms_phong(shader, ViewMatrix, scene);

			Material& m = instances[i].material;
			glUniform4fv(shader->loc_material_ambient, 1, &m.ambient[0]);
			glUniform4fv(shader->loc_material_diffuse, 1, &m.diffuse[0]);
			glUniform4fv(shader->loc_material_specular, 1, &m.specular[0]);
			glUniform1f(shader->loc_material_shininess, m.exponent);
			break;
		}
		case SHADER_GOURAUD_TEXTURE: {
			auto* shader = static_cast<Shader_Gouraud_Texture*>(&shader_list[shader_ID_mapper[actual_shader]].get());
			glUseProgram(shader->h_ShaderProgram);

			glm::mat4 ModelMatrix = instances[i].ModelMatrix;
			glm::mat4 MV = ViewMatrix * ModelMatrix;
			glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));
			glm::mat4 MVP = ProjectionMatrix * MV;

			glUniformMatrix4fv(shader->loc_ModelViewMatrix, 1, GL_FALSE, &MV[0][0]);
			glUniformMatrix4fv(shader->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix3fv(shader->loc_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);

			// 다중 조명 설정
			set_lighting_uniforms_gouraud_texture(shader, ViewMatrix, scene);

			Material& m = instances[i].material;
			glUniform4fv(shader->loc_material_ambient, 1, &m.ambient[0]);
			glUniform4fv(shader->loc_material_diffuse, 1, &m.diffuse[0]);
			glUniform4fv(shader->loc_material_specular, 1, &m.specular[0]);
			glUniform1f(shader->loc_material_shininess, m.exponent);

			// 텍스처 설정
			if (instances[i].use_texture && instances[i].texture_id != 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, instances[i].texture_id);
				glUniform1i(shader->loc_texture_sampler, 0);
				glUniform1i(shader->loc_use_texture, 1);
			}
			else {
				glUniform1i(shader->loc_use_texture, 0);
			}
			break;
		}
		case SHADER_PHONG_TEXTURE: {
			auto* shader = static_cast<Shader_Phong_Texture*>(&shader_list[shader_ID_mapper[actual_shader]].get());
			glUseProgram(shader->h_ShaderProgram);

			glm::mat4 ModelMatrix = instances[i].ModelMatrix;
			glm::mat4 MV = ViewMatrix * ModelMatrix;
			glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));
			glm::mat4 MVP = ProjectionMatrix * MV;

			glUniformMatrix4fv(shader->loc_ModelViewMatrix, 1, GL_FALSE, &MV[0][0]);
			glUniformMatrix4fv(shader->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix3fv(shader->loc_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);

			// 다중 조명 설정
			set_lighting_uniforms_phong_texture(shader, ViewMatrix, scene);

			Material& m = instances[i].material;
			glUniform4fv(shader->loc_material_ambient, 1, &m.ambient[0]);
			glUniform4fv(shader->loc_material_diffuse, 1, &m.diffuse[0]);
			glUniform4fv(shader->loc_material_specular, 1, &m.specular[0]);
			glUniform1f(shader->loc_material_shininess, m.exponent);

			// 텍스처 설정
			if (instances[i].use_texture && instances[i].texture_id != 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, instances[i].texture_id);
				glUniform1i(shader->loc_texture_sampler, 0);
				glUniform1i(shader->loc_use_texture, 1);
			}
			else {
				glUniform1i(shader->loc_use_texture, 0);
			}
			break;
		}
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3 * n_triangles);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
}