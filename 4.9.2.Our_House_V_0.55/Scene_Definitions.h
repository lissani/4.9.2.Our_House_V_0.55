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
	STATIC_OBJECT_BIKE, STATIC_OBJECT_TEAPOT
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

// 다중 조명을 지원하는 Gouraud 쉐이더
struct Shader_Gouraud : Shader {
	GLint loc_ModelViewMatrix;
	GLint loc_NormalMatrix;

	// 다중 조명을 위한 uniform 위치들
	GLint loc_world_light_position;
	GLint loc_world_light_color;
	GLint loc_world_light_enabled;

	GLint loc_view_light_position;
	GLint loc_view_light_color;
	GLint loc_view_light_enabled;

	GLint loc_model_light_position;
	GLint loc_model_light_color;
	GLint loc_model_light_enabled;

	GLint loc_material_ambient;
	GLint loc_material_diffuse;
	GLint loc_material_specular;
	GLint loc_material_shininess;
	void prepare_shader();
};

// 다중 조명을 지원하는 Phong 쉐이더
struct Shader_Phong : Shader {
	GLint loc_ModelViewMatrix;
	GLint loc_NormalMatrix;

	// 다중 조명을 위한 uniform 위치들
	GLint loc_world_light_position;
	GLint loc_world_light_color;
	GLint loc_world_light_enabled;

	GLint loc_view_light_position;
	GLint loc_view_light_color;
	GLint loc_view_light_enabled;

	GLint loc_model_light_position;
	GLint loc_model_light_color;
	GLint loc_model_light_enabled;

	GLint loc_material_ambient;
	GLint loc_material_diffuse;
	GLint loc_material_specular;
	GLint loc_material_shininess;
	void prepare_shader();
};

// 다중 조명을 지원하는 Gouraud Texture 쉐이더
struct Shader_Gouraud_Texture : Shader {
	GLint loc_ModelViewMatrix;
	GLint loc_NormalMatrix;

	// 다중 조명을 위한 uniform 위치들
	GLint loc_world_light_position;
	GLint loc_world_light_color;
	GLint loc_world_light_enabled;

	GLint loc_view_light_position;
	GLint loc_view_light_color;
	GLint loc_view_light_enabled;

	GLint loc_model_light_position;
	GLint loc_model_light_color;
	GLint loc_model_light_enabled;

	GLint loc_material_ambient;
	GLint loc_material_diffuse;
	GLint loc_material_specular;
	GLint loc_material_shininess;

	GLint loc_texture_sampler;
	GLint loc_use_texture;

	void prepare_shader() override;
};

// 다중 조명을 지원하는 Phong Texture 쉐이더
struct Shader_Phong_Texture : Shader {
	GLint loc_ModelViewMatrix;
	GLint loc_NormalMatrix;

	// 다중 조명을 위한 uniform 위치들
	GLint loc_world_light_position;
	GLint loc_world_light_color;
	GLint loc_world_light_enabled;

	GLint loc_view_light_position;
	GLint loc_view_light_color;
	GLint loc_view_light_enabled;

	GLint loc_model_light_position;
	GLint loc_model_light_color;
	GLint loc_model_light_enabled;

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

// 전방 선언 (Scene 구조체를 참조하기 위해)
struct Scene;

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
	// Scene 객체를 받도록 수정
	void draw_object(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
		std::vector<std::reference_wrapper<Shader>>& shader_list, Scene& scene);
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

struct Teapot : public Static_Object {
private:
	float rotation_angle; // 회전 각도
	glm::vec3 center_position; // 무게중심 위치

public:
	Teapot(STATIC_OBJECT_ID _object_id) : Static_Object(_object_id), rotation_angle(0.0f) {}
	void define_object();
	void update_animation(float delta_time); // 애니메이션 업데이트 메서드 추가

	// 투명 렌더링을 위한 draw_object 오버라이드
	void draw_object(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
		std::vector<std::reference_wrapper<Shader>>& shader_list, Scene& scene);

private:
	void draw_with_transparency(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
		std::vector<std::reference_wrapper<Shader>>& shader_list, const Scene& scene);
};

struct Static_Geometry_Data {
	Building building{ STATIC_OBJECT_BUILDING };
	Ant ant{ STATIC_OBJECT_ANT };
	Wood_Tower wood_tower{ STATIC_OBJECT_WOOD_TOWER };
	Cat cat{ STATIC_OBJECT_CAT };
	Iron_Man iron_man{ STATIC_OBJECT_IRON_MAN };
	Bike bike{ STATIC_OBJECT_BIKE };
	Teapot teapot{ STATIC_OBJECT_TEAPOT };
};

struct Dynamic_Object {
	DYNAMIC_OBJECT_ID object_id;
	std::vector<Static_Object> object_frames;
	bool flag_valid;

	Dynamic_Object() {}
	Dynamic_Object(DYNAMIC_OBJECT_ID _object_id) : object_id(_object_id) {
		object_frames.clear();
	}

	// Scene 객체를 받도록 수정
	void draw_object(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
		std::vector<std::reference_wrapper<Shader>>& shader_list, int time_stamp, Scene& scene);
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

// 조명 정보 구조체
struct LightInfo {
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	bool enabled;

	LightInfo() : position(0.0f), color(1.0f), intensity(1.0f), enabled(false) {}
	LightInfo(glm::vec3 pos, glm::vec3 col, float inten)
		: position(pos), color(col), intensity(inten), enabled(true) {
	}
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

	// 조명 시스템
	bool world_light_enabled = true;   // 세상좌표계 광원
	bool view_light_enabled = false;   // 눈좌표계 광원
	bool model_light_enabled = false;  // 모델좌표계 광원

	bool transparency_enabled;     // '6' 키: 투명도 on/off
	float transparency_alpha;

	LightInfo world_light;    // 세상좌표계 기준 광원
	LightInfo view_light;     // 눈좌표계 기준 광원
	LightInfo model_light;    // 모델좌표계 기준 광원

	Axis_Object axis_object;

	Scene() {
		time_stamp = 0;
		static_objects.clear();
		shader_list.clear();
		shader_kind = SHADER_SIMPLE;
		ViewMatrix = ProjectionMatrix = glm::mat4(1.0f);

		// 조명 초기화
		world_light = LightInfo(glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
		view_light = LightInfo(glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.8f, 0.8f, 1.0f), 0.8f);
		model_light = LightInfo(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(1.0f, 0.8f, 0.6f), 0.9f);
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