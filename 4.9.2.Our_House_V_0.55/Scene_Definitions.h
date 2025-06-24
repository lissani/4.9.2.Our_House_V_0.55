#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shaders/LoadShaders.h"
#include "Camera.h"
#include "Texture_Manager.h"

#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))
#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

#define N_MAX_STATIC_OBJECTS		10
#define N_MAX_DYNAMIC_OBJECTS		10
#define N_MAX_CAMERAS		10
#define N_MAX_SHADERS		10

extern unsigned int static_object_ID_mapper[N_MAX_STATIC_OBJECTS];
extern unsigned int dynamic_object_ID_mapper[N_MAX_DYNAMIC_OBJECTS];
extern unsigned int camera_ID_mapper[N_MAX_CAMERAS];
extern unsigned int shader_ID_mapper[N_MAX_SHADERS];

enum STATIC_OBJECT_ID {
	STATIC_OBJECT_BUILDING = 0,
	STATIC_OBJECT_ANT, STATIC_OBJECT_WOOD_TOWER,
	STATIC_OBJECT_CAT, STATIC_OBJECT_IRON_MAN,
	STATIC_OBJECT_BIKE
};

enum DYNAMIC_OBJECT_ID {
	DYNAMIC_OBJECT_WOLF = 0, DYNAMIC_OBJECT_SPIDER
};

enum SHADER_ID {
	SHADER_SIMPLE = 0, SHADER_GOURAUD, SHADER_PHONG,
	SHADER_GOURAUD_TEXTURE, SHADER_PHONG_TEXTURE, NUM_SHADERS
};

struct Shader {
	ShaderInfo shader_info[3];
	GLuint h_ShaderProgram;
	GLint loc_ModelViewProjectionMatrix;

	Shader() {
		h_ShaderProgram = NULL;
		loc_ModelViewProjectionMatrix = -1;
		shader_info[0] = shader_info[1] = shader_info[2] = { NULL, NULL };
	}
	virtual void prepare_shader() = 0;
};

struct Shader_Simple : Shader {
	GLint loc_primitive_color;
	void prepare_shader();
};

struct Shader_Gouraud : Shader {
	GLint loc_ModelViewMatrix;
	GLint loc_NormalMatrix;
	GLint loc_light_position;
	GLint loc_view_position;

	GLint loc_material_ambient;
	GLint loc_material_diffuse;
	GLint loc_material_specular;
	GLint loc_material_shininess;
	void prepare_shader();
};

struct Shader_Phong : Shader_Gouraud {
	void prepare_shader();
};

// 텍스처 쉐이더들을 Scene_Definitions.h에 추가
struct Shader_Gouraud_Texture : Shader {
	GLint loc_ModelViewMatrix;
	GLint loc_NormalMatrix;
	GLint loc_light_position;
	GLint loc_view_position;

	GLint loc_material_ambient;
	GLint loc_material_diffuse;
	GLint loc_material_specular;
	GLint loc_material_shininess;

	GLint loc_texture_sampler;
	GLint loc_use_texture;

	void prepare_shader() override;
};

struct Shader_Phong_Texture : Shader {
	GLint loc_ModelViewMatrix;
	GLint loc_NormalMatrix;
	GLint loc_light_position;
	GLint loc_view_position;

	GLint loc_material_ambient;
	GLint loc_material_diffuse;
	GLint loc_material_specular;
	GLint loc_material_shininess;

	GLint loc_texture_sampler;
	GLint loc_use_texture;

	void prepare_shader() override;
};

struct Shader_Data {
	Shader_Simple shader_simple;
	Shader_Gouraud shader_gouraud;
	Shader_Phong shader_phong;
	Shader_Gouraud_Texture shader_gouraud_texture;
	Shader_Phong_Texture shader_phong_texture;
};

struct Material {
	glm::vec4 emission, ambient, diffuse, specular;
	GLfloat exponent;
};

struct Instance {
	glm::mat4 ModelMatrix;
	Material material;
	GLuint texture_id;
	bool use_texture;

	Instance() : ModelMatrix(1.0f), texture_id(0), use_texture(false) {}
};

struct Axis_Object {
	GLuint VBO, VAO;
	GLfloat vertices_axes[6][3] = {
		{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f }
	};
	GLfloat axes_color[3][3] = { { 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } };

	void define_axis();
	void draw_axis(Shader_Simple* shader_simple, glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix);
	void draw_camera_axis(Shader_Simple* shader_simple,
		glm::mat4& ViewMatrix,
		glm::mat4& ProjectionMatrix,
		glm::vec3 position,
		glm::vec3 uaxis,
		glm::vec3 vaxis,
		glm::vec3 naxis,
		float axis_length = 30.0f);
};

struct Static_Object {
	STATIC_OBJECT_ID object_id;
	char filename[512];
	int n_fields;
	int n_triangles;
	GLfloat* vertices;
	GLuint VBO, VAO;
	GLenum front_face_mode;

	std::vector<Instance> instances;
	bool flag_valid;
	bool force_texture_shader;

	Static_Object() : force_texture_shader(false) {}
	Static_Object(STATIC_OBJECT_ID _object_id) : object_id(_object_id) {
		instances.clear();
	}
	void read_geometry(int bytes_per_primitive);
	void prepare_geom_of_static_object();
	void draw_object(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
		std::vector<std::reference_wrapper<Shader>>& shader_list);
};

struct Building : public Static_Object {
	Building(STATIC_OBJECT_ID _object_id) : Static_Object(_object_id) {}
	void define_object();
};

struct Ant : public Static_Object {
	Ant(STATIC_OBJECT_ID _object_id) : Static_Object(_object_id) {}
	void define_object();
};

struct Wood_Tower : public Static_Object {
	Wood_Tower(STATIC_OBJECT_ID _object_id) : Static_Object(_object_id) {}
	void define_object();
};

struct Cat : public Static_Object {
	Cat(STATIC_OBJECT_ID _object_id) : Static_Object(_object_id) {}
	void define_object();
};

struct Iron_Man : public Static_Object {
	Iron_Man(STATIC_OBJECT_ID _object_id) : Static_Object(_object_id) {}
	void define_object();
};

struct Bike : public Static_Object {
	Bike(STATIC_OBJECT_ID _object_id) : Static_Object(_object_id) {}
	void define_object();
};

struct Static_Geometry_Data {
	Building building{ STATIC_OBJECT_BUILDING };
	Ant ant{ STATIC_OBJECT_ANT };
	Wood_Tower wood_tower{ STATIC_OBJECT_WOOD_TOWER };
	Cat cat{ STATIC_OBJECT_CAT };
	Iron_Man iron_man{ STATIC_OBJECT_IRON_MAN };
	Bike bike{ STATIC_OBJECT_BIKE };
};

struct Dynamic_Object {
	DYNAMIC_OBJECT_ID object_id;
	std::vector<Static_Object> object_frames;
	bool flag_valid;

	Dynamic_Object() {}
	Dynamic_Object(DYNAMIC_OBJECT_ID _object_id) : object_id(_object_id) {
		object_frames.clear();
	}

	void draw_object(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
		std::vector<std::reference_wrapper<Shader>>& shader_list, int time_stamp);
};

struct Wolf_D : public Dynamic_Object {
	Wolf_D(DYNAMIC_OBJECT_ID _object_id) : Dynamic_Object(_object_id) {}
	void define_object();
};

struct Spider_D : public Dynamic_Object {
	Spider_D(DYNAMIC_OBJECT_ID _object_id) : Dynamic_Object(_object_id) {}
	void define_object();
};

struct Dynamic_Geometry_Data {
	Wolf_D wolf_d{ DYNAMIC_OBJECT_WOLF };
	Spider_D spider_d{ DYNAMIC_OBJECT_SPIDER };
};

struct Window {
	int width, height;
	float aspect_ratio;
};

struct Scene {
	unsigned int time_stamp;
	Static_Geometry_Data static_geometry_data;
	std::vector<std::reference_wrapper<Static_Object>> static_objects;

	Dynamic_Geometry_Data dynamic_geometry_data;
	std::vector<std::reference_wrapper<Dynamic_Object>> dynamic_objects;

	Camera_Data camera_data;
	std::vector<std::reference_wrapper<Camera>> camera_list;

	Shader_Data shader_data;
	std::vector<std::reference_wrapper<Shader>> shader_list;
	SHADER_ID shader_kind;

	Window window;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	bool show_camera_frames = false;

	Axis_Object axis_object;

	Scene() {
		time_stamp = 0;
		static_objects.clear();
		shader_list.clear();
		shader_kind = SHADER_SIMPLE;
		ViewMatrix = ProjectionMatrix = glm::mat4(1.0f);
	}

	void clock(int clock_id);
	void build_static_world();
	void build_dynamic_world();
	void create_camera_list(int win_width, int win_height, float win_aspect_ratio);
	void build_shader_list();
	void initialize();
	void draw_static_world();
	void draw_dynamic_world();
	void draw_axis();
	void draw_camera_frames();
	void draw_world();
};