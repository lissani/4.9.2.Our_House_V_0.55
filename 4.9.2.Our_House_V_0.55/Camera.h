#pragma once

#include <glm/gtc/matrix_transform.hpp>

enum Camera_Projection_TYPE {
	CAMERA_PROJECTION_PERSPECTIVE = 0, CAMERA_PROJECTION_ORTHOGRAPHIC
};

enum Camera_ID {
	CAMERA_MAIN = 0,
	CAMERA_CC_0, CAMERA_CC_1,
	CAMERA_CC_2, CAMERA_CC_DYNAMIC,
	CAMERA_SIDE_FRONT, CAMERA_TOP, CAMERA_SIDE
};

struct Camera_View {
	glm::vec3 pos;
	glm::vec3 uaxis, vaxis, naxis;
};
 
struct Camera_Projection {
	Camera_Projection_TYPE projection_type;
	union {
		struct {
			float fovy, aspect, n, f;
		} pers;
		struct {
			float left, right, bottom, top, n, f;
		} ortho;
	} params;
};

struct View_Port {
	float x, y, w, h;
};

struct Camera {
	Camera_ID camera_id;
	Camera_View cam_view;
	glm::mat4 ViewMatrix;
	Camera_Projection cam_proj;
	glm::mat4 ProjectionMatrix;
	View_Port view_port;

	Camera(Camera_ID _camera_id) : camera_id(_camera_id) {}
	bool flag_valid;
	bool flag_move; // what else?
};

struct Perspective_Camera : public Camera { // 주 camera, cctv camera
	Perspective_Camera(Camera_ID _camera_id) : Camera(_camera_id) {}
	void define_camera(int win_width, int win_height, float win_aspect_ratio);
};

struct Orthographic_Camera : public Camera { // 평면도, 상면도 ...
	Orthographic_Camera(Camera_ID _camera_id) : Camera(_camera_id) {}
	void define_camera(int win_width, int win_height, float win_aspect_ratio);
};

struct Camera_Data {
	Perspective_Camera cam_main { CAMERA_MAIN };
	Perspective_Camera cam_cc_0 { CAMERA_CC_0 };
	Perspective_Camera cam_cc_1 { CAMERA_CC_1 };
	Perspective_Camera cam_cc_2 { CAMERA_CC_2 };
	Perspective_Camera cam_cc_dyn { CAMERA_CC_DYNAMIC };

	Orthographic_Camera cam_side_front { CAMERA_SIDE_FRONT };
	Orthographic_Camera cam_top { CAMERA_TOP };
	Orthographic_Camera cam_side { CAMERA_SIDE };
};