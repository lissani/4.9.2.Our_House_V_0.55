#define _CRT_SECURE_NO_WARNINGS

#include "Scene_Definitions.h"

void Static_Object::read_geometry(int bytes_per_primitive) {
	FILE* fp;

	// fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
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
	fread(vertices, bytes_per_primitive, n_triangles, fp); // assume the data file has no faults.
	// fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);
}

void Static_Object::prepare_geom_of_static_object() {
	int i, n_bytes_per_vertex, n_bytes_per_triangle;
	char filename[512];

	n_bytes_per_vertex = n_fields * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	read_geometry(n_bytes_per_triangle);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, n_triangles * n_bytes_per_triangle, vertices, GL_STATIC_DRAW);

	free(vertices);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, (GLvoid*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

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

/// my static objects
void Ant::define_object() {
	glm::mat4* cur_MM; // �𵨸� ��ȯ
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
	cur_material->exponent = 128.0f * 0.078125f;;
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

void print_mat4(const char* string, glm::mat4 M) {
	fprintf(stdout, "\n***** %s ******\n", string);
	for (int i = 0; i < 4; i++)
		fprintf(stdout, "*** COL[%d] (%f, %f, %f, %f)\n", i, M[i].x, M[i].y, M[i].z, M[i].w);
	fprintf(stdout, "**************\n\n");
}

void Static_Object::draw_object(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
	std::vector<std::reference_wrapper<Shader>>& shader_list) {
	glm::mat4 ModelViewProjectionMatrix;
	glFrontFace(front_face_mode);

	for (int i = 0; i < instances.size(); i++) {
		ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * instances[i].ModelMatrix;
		switch (shader_kind) {
		case SHADER_SIMPLE: {
			Shader_Simple* shader_simple_ptr = static_cast<Shader_Simple*>(&shader_list[shader_ID_mapper[shader_kind]].get());
			glUseProgram(shader_simple_ptr->h_ShaderProgram);
			glUniformMatrix4fv(shader_simple_ptr->loc_ModelViewProjectionMatrix, 1, GL_FALSE,
				&ModelViewProjectionMatrix[0][0]);
			glUniform3f(shader_simple_ptr->loc_primitive_color, instances[i].material.diffuse.r,
				instances[i].material.diffuse.g, instances[i].material.diffuse.b);
			break;
		}
		case SHADER_GOURAUD: {
			auto* shader = static_cast<Shader_Gouraud*>(&shader_list[shader_ID_mapper[shader_kind]].get());
			glUseProgram(shader->h_ShaderProgram);

			glm::mat4 ModelMatrix = instances[i].ModelMatrix;
			glm::mat4 MV = ViewMatrix * ModelMatrix;
			glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));
			glm::mat4 MVP = ProjectionMatrix * MV;

			glm::vec3 light_position_world = glm::vec3(0.0f, 0.0f, 100.0f);
			glm::vec3 light_position_view = glm::vec3(ViewMatrix * glm::vec4(light_position_world, 1.0f));
			glm::vec3 view_position_view = glm::vec3(0.0f);

			glUniformMatrix4fv(shader->loc_ModelViewMatrix, 1, GL_FALSE, &MV[0][0]);
			glUniformMatrix4fv(shader->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix3fv(shader->loc_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);

			glUniform3fv(shader->loc_light_position, 1, &light_position_view[0]);
			glUniform3fv(shader->loc_view_position, 1, &view_position_view[0]);

			Material& m = instances[i].material;
			glUniform4fv(shader->loc_material_ambient, 1, &m.ambient[0]);
			glUniform4fv(shader->loc_material_diffuse, 1, &m.diffuse[0]);
			glUniform4fv(shader->loc_material_specular, 1, &m.specular[0]);
			glUniform1f(shader->loc_material_shininess, m.exponent);
			break;
		}
		case SHADER_PHONG: {
			auto* shader = static_cast<Shader_Phong*>(&shader_list[shader_ID_mapper[shader_kind]].get());
			glUseProgram(shader->h_ShaderProgram);

			glm::mat4 ModelMatrix = instances[i].ModelMatrix;
			glm::mat4 MV = ViewMatrix * ModelMatrix;
			glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));
			glm::mat4 MVP = ProjectionMatrix * MV;

			glm::vec3 light_position_world = glm::vec3(0.0f, 0.0f, 100.0f);
			glm::vec3 light_position_view = glm::vec3(ViewMatrix * glm::vec4(light_position_world, 1.0f));
			glm::vec3 view_position_view = glm::vec3(0.0f);

			glUniformMatrix4fv(shader->loc_ModelViewMatrix, 1, GL_FALSE, &MV[0][0]);
			glUniformMatrix4fv(shader->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix3fv(shader->loc_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);

			glUniform3fv(shader->loc_light_position, 1, &light_position_view[0]);
			glUniform3fv(shader->loc_view_position, 1, &view_position_view[0]);

			Material& m = instances[i].material;
			glUniform4fv(shader->loc_material_ambient, 1, &m.ambient[0]);
			glUniform4fv(shader->loc_material_diffuse, 1, &m.diffuse[0]);
			glUniform4fv(shader->loc_material_specular, 1, &m.specular[0]);
			glUniform1f(shader->loc_material_shininess, m.exponent);
			break;
		}
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3 * n_triangles);
		glBindVertexArray(0);
		glUseProgram(0);
	}
}
