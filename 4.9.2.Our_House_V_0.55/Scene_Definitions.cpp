#define _CRT_SECURE_NO_WARNINGS

#include "Scene_Definitions.h"

unsigned int static_object_ID_mapper[N_MAX_STATIC_OBJECTS];
unsigned int dynamic_object_ID_mapper[N_MAX_DYNAMIC_OBJECTS];
unsigned int camera_ID_mapper[N_MAX_CAMERAS];
unsigned int shader_ID_mapper[N_MAX_SHADERS];

void Axis_Object::define_axis() {
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_axes), &vertices_axes[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Axis_Object::draw_axis(Shader_Simple* shader_simple, glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix) {
#define WC_AXIS_LENGTH		60.0f
	glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(WC_AXIS_LENGTH, WC_AXIS_LENGTH, WC_AXIS_LENGTH));
	glm::mat4 ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUseProgram(shader_simple->h_ShaderProgram);
	glUniformMatrix4fv(shader_simple->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);


	glBindVertexArray(VAO);
	glUniform3fv(shader_simple->loc_primitive_color, 1, axes_color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3fv(shader_simple->loc_primitive_color, 1, axes_color[1]);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3fv(shader_simple->loc_primitive_color, 1, axes_color[2]);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(0);
	glUseProgram(0);
}
void Axis_Object::draw_camera_axis(Shader_Simple* shader_simple,
	glm::mat4& ViewMatrix,
	glm::mat4& ProjectionMatrix,
	glm::vec3 position,
	glm::vec3 uaxis,
	glm::vec3 vaxis,
	glm::vec3 naxis,
	float axis_length) {

	// ȸ�� ��� ����
	glm::mat4 RotationMatrix = glm::mat4(1.0f);
	RotationMatrix[0] = glm::vec4(glm::normalize(uaxis), 0.0f);    // X��
	RotationMatrix[1] = glm::vec4(glm::normalize(vaxis), 0.0f);    // Y��  
	RotationMatrix[2] = glm::vec4(glm::normalize(-naxis), 0.0f);   // Z��
	RotationMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// ��ȯ ��� ����: T * R * S
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(axis_length));
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	glm::mat4 ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUseProgram(shader_simple->h_ShaderProgram);
	glUniformMatrix4fv(shader_simple->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	glBindVertexArray(VAO);
	glUniform3fv(shader_simple->loc_primitive_color, 1, axes_color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3fv(shader_simple->loc_primitive_color, 1, axes_color[1]);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3fv(shader_simple->loc_primitive_color, 1, axes_color[2]);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Scene::draw_camera_frames() {
	if (!show_camera_frames) return;

	static bool debug_printed = false;  // ���� ������ �� ���� ���

	Shader_Simple* shader_simple_ptr = static_cast<Shader_Simple*>(&shader_list[shader_ID_mapper[SHADER_SIMPLE]].get());

	for (const auto& camera_ref : camera_list) {
		const Camera& cam = camera_ref.get();
		if (!cam.flag_valid) continue;

		// �� ���� ����� ���
		if (!debug_printed) {
			printf("Camera pos: (%.2f, %.2f, %.2f)\n",
				cam.cam_view.pos.x, cam.cam_view.pos.y, cam.cam_view.pos.z);
			printf("U-axis: (%.2f, %.2f, %.2f)\n",
				cam.cam_view.uaxis.x, cam.cam_view.uaxis.y, cam.cam_view.uaxis.z);
			printf("V-axis: (%.2f, %.2f, %.2f)\n",
				cam.cam_view.vaxis.x, cam.cam_view.vaxis.y, cam.cam_view.vaxis.z);
			printf("N-axis: (%.2f, %.2f, %.2f)\n",
				cam.cam_view.naxis.x, cam.cam_view.naxis.y, cam.cam_view.naxis.z);
			printf("---\n");
		}

		axis_object.draw_camera_axis(shader_simple_ptr, ViewMatrix, ProjectionMatrix,
			cam.cam_view.pos,
			cam.cam_view.uaxis,
			cam.cam_view.vaxis,
			cam.cam_view.naxis,
			30.0f);
	}

	debug_printed = true;  // ù ��° ȣ�� �� �÷��� ����
}

void Scene::clock(int clock_id) { // currently one clock
	time_stamp = ++time_stamp % UINT_MAX;
}

void Scene::build_static_world() {
	static_geometry_data.building.define_object();
	static_object_ID_mapper[STATIC_OBJECT_BUILDING] = static_objects.size();
	static_objects.push_back(static_geometry_data.building);

	static_geometry_data.ant.define_object();
	static_object_ID_mapper[STATIC_OBJECT_ANT] = static_objects.size();
	static_objects.push_back(static_geometry_data.ant);

	static_geometry_data.wood_tower.define_object();
	static_object_ID_mapper[STATIC_OBJECT_WOOD_TOWER] = static_objects.size();
	static_objects.push_back(static_geometry_data.wood_tower);

	static_geometry_data.cat.define_object();
	static_object_ID_mapper[STATIC_OBJECT_CAT] = static_objects.size();
	static_objects.push_back(static_geometry_data.cat);

	static_geometry_data.iron_man.define_object();
	static_object_ID_mapper[STATIC_OBJECT_IRON_MAN] = static_objects.size();
	static_objects.push_back(static_geometry_data.iron_man);

	static_geometry_data.bike.define_object();
	static_object_ID_mapper[STATIC_OBJECT_BIKE] = static_objects.size();
	static_objects.push_back(static_geometry_data.bike);
}

void Scene::build_dynamic_world() {

	dynamic_geometry_data.wolf_d.define_object();
	dynamic_object_ID_mapper[DYNAMIC_OBJECT_WOLF] = dynamic_objects.size();
	dynamic_objects.push_back(dynamic_geometry_data.wolf_d);

	dynamic_geometry_data.spider_d.define_object();
	dynamic_object_ID_mapper[DYNAMIC_OBJECT_SPIDER] = dynamic_objects.size();
	dynamic_objects.push_back(dynamic_geometry_data.spider_d);
}

void Scene::create_camera_list(int win_width, int win_height, float win_aspect_ratio) {
	camera_list.clear();
	// main camera
	camera_data.cam_main.define_camera(win_width, win_height, win_aspect_ratio);
	camera_ID_mapper[CAMERA_MAIN] = camera_list.size();
	camera_list.push_back(camera_data.cam_main);

	camera_data.cam_cc_0.define_camera(win_width, win_height, win_aspect_ratio);
	camera_ID_mapper[CAMERA_CC_0] = camera_list.size();
	camera_list.push_back(camera_data.cam_cc_0);

	camera_data.cam_cc_1.define_camera(win_width, win_height, win_aspect_ratio);
	camera_ID_mapper[CAMERA_CC_1] = camera_list.size();
	camera_list.push_back(camera_data.cam_cc_1);

	camera_data.cam_cc_2.define_camera(win_width, win_height, win_aspect_ratio);
	camera_ID_mapper[CAMERA_CC_2] = camera_list.size();
	camera_list.push_back(camera_data.cam_cc_2);

	camera_data.cam_cc_dyn.define_camera(win_width, win_height, win_aspect_ratio);
	camera_ID_mapper[CAMERA_CC_DYNAMIC] = camera_list.size();
	camera_list.push_back(camera_data.cam_cc_dyn);

	camera_data.cam_side_front.define_camera(win_width, win_height, win_aspect_ratio);
	camera_ID_mapper[CAMERA_SIDE_FRONT] = camera_list.size();
	camera_list.push_back(camera_data.cam_side_front);

	camera_data.cam_top.define_camera(win_width, win_height, win_aspect_ratio);
	camera_ID_mapper[CAMERA_TOP] = camera_list.size();
	camera_list.push_back(camera_data.cam_top);

	camera_data.cam_side.define_camera(win_width, win_height, win_aspect_ratio);
	camera_ID_mapper[CAMERA_SIDE] = camera_list.size();
	camera_list.push_back(camera_data.cam_side);

}

void Scene::build_shader_list() {
	shader_data.shader_simple.prepare_shader();
	shader_ID_mapper[SHADER_SIMPLE] = shader_list.size();
	shader_list.push_back(shader_data.shader_simple);

	shader_data.shader_gouraud.prepare_shader();
	shader_ID_mapper[SHADER_GOURAUD] = shader_list.size();
	shader_list.push_back(shader_data.shader_gouraud);

	shader_data.shader_phong.prepare_shader();
	shader_ID_mapper[SHADER_PHONG] = shader_list.size();
	shader_list.push_back(shader_data.shader_phong);
}

void Scene::initialize() {
	axis_object.define_axis();
	build_static_world();
	build_dynamic_world();
	create_camera_list(window.width, window.height, window.aspect_ratio);
	build_shader_list();
}

void Scene::draw_static_world() {
	glm::mat4 ModelViewProjectionMatrix;
	for (auto static_object = static_objects.begin(); static_object != static_objects.end(); static_object++) {
		if (static_object->get().flag_valid == false) continue;
		/*
		SHADER_ID shader = shader_kind;
		if (static_object->get().object_id == STATIC_OBJECT_IRON_MAN)
			shader = ironman_shader_mode;
		*/
		static_object->get().draw_object(ViewMatrix, ProjectionMatrix, shader_kind, shader_list);
	}
}

void Scene::draw_dynamic_world() {
	glm::mat4 ModelViewProjectionMatrix;
	for (auto dynamic_object = dynamic_objects.begin(); dynamic_object != dynamic_objects.end(); dynamic_object++) {
		if (dynamic_object->get().flag_valid == false) continue;
		dynamic_object->get().draw_object(ViewMatrix, ProjectionMatrix, shader_kind, shader_list, time_stamp);
	}
}

void Scene::draw_axis() {
	axis_object.draw_axis(static_cast<Shader_Simple*>(&shader_list[shader_ID_mapper[SHADER_SIMPLE]].get()),
		ViewMatrix, ProjectionMatrix);
}

void Scene::draw_world() {
	draw_axis();
	draw_static_world();
	draw_dynamic_world();
	draw_camera_frames();
}