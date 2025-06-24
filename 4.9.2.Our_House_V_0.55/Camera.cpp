#define _CRT_SECURE_NO_WARNINGS

#include "Camera.h"
#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

void Perspective_Camera::define_camera(int win_width, int win_height, float win_aspect_ratio) {
	glm::mat3 R33_t;
	glm::mat4 T;
 
	switch (camera_id) {
	case CAMERA_MAIN: {
		flag_valid = true;
		flag_move = true; // yes. the main camera is permitted to move

		// let's use glm funtions to set up the initial camera pose
		ViewMatrix = glm::lookAt(glm::vec3(-600.0f, -600.0f, 400.0f), glm::vec3(125.0f, 80.0f, 25.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)); // initial pose for main camera
		// 카메라의 위치, 카메라가 세상을 바라보는 방향, 영상의 위쪽 방향
		cam_view.uaxis = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		cam_view.vaxis = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		cam_view.naxis = glm::vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);
		// 카메라 속성들이 계속해서 변화하는 동적 cam이므로
		// 역계산을 통한 pos 다시 계산해야 함
		R33_t = glm::transpose(glm::mat3(ViewMatrix));
		T = glm::mat4(R33_t) * ViewMatrix;
		cam_view.pos = -glm::vec3(T[3][0], T[3][1], T[3][2]); // why does this work?

		cam_proj.projection_type = CAMERA_PROJECTION_PERSPECTIVE;
		cam_proj.params.pers.fovy = 15.0f * TO_RADIAN;
		cam_proj.params.pers.aspect = win_aspect_ratio;
		cam_proj.params.pers.n = 1.0f;
		cam_proj.params.pers.f = 50000.0f;

		ProjectionMatrix = glm::perspective(cam_proj.params.pers.fovy, cam_proj.params.pers.aspect,
			cam_proj.params.pers.n, cam_proj.params.pers.f);
		//view_port.x = 200; view_port.y = 200; view_port.w = win_width - 200; view_port.h = win_height - 200;

		view_port.x = 220;
		view_port.y = 200;
		view_port.w = win_width - 400 - 220;
		view_port.h = win_height - 200;
		break;
	}
	case CAMERA_CC_0: {
		flag_valid = true;
		flag_move = false; // yes. the main camera is permitted to move

		// let's use glm funtions to set up the initial camera pose
		cam_view.pos = glm::vec3(40.0f, 40.0f, 45.0f);
		glm::vec3 target = glm::vec3(25.0f, 25.0f, 10.0f);
		ViewMatrix = glm::lookAt(cam_view.pos, target, glm::vec3(0.0f, 0.0f, 1.0f)); // initial pose for main camera
		// 카메라의 위치, 카메라가 세상을 바라보는 방향, 영상의 위쪽 방향
		cam_view.uaxis = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		cam_view.vaxis = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		cam_view.naxis = glm::vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);
		
		cam_proj.projection_type = CAMERA_PROJECTION_PERSPECTIVE;
		cam_proj.params.pers.fovy = 65.0f * TO_RADIAN; // 카메라 시야각
		cam_proj.params.pers.aspect = win_aspect_ratio;
		cam_proj.params.pers.n = 1.0f;
		cam_proj.params.pers.f = 1000.0f;

		ProjectionMatrix = glm::perspective(cam_proj.params.pers.fovy, cam_proj.params.pers.aspect,
			cam_proj.params.pers.n, cam_proj.params.pers.f);
		// 뷰포트 - 화면에 도시
		view_port.x = 10;
		view_port.y = win_height - 10 - 150;
		view_port.w = 200;
		view_port.h = 150;
		break;
	}
	case CAMERA_CC_1: {
		flag_valid = true;
		flag_move = false; // yes. the main camera is permitted to move

		// let's use glm funtions to set up the initial camera pose
		cam_view.pos = glm::vec3(100.0f, 75.0f, 45.0f);
		glm::vec3 target = glm::vec3(100.0f, 120.0f, 10.0f);
		ViewMatrix = glm::lookAt(cam_view.pos, target, glm::vec3(0.0f, 0.0f, 1.0f)); // initial pose for main camera
		// 카메라의 위치, 카메라가 세상을 바라보는 방향, 영상의 위쪽 방향
		cam_view.uaxis = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		cam_view.vaxis = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		cam_view.naxis = glm::vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);

		cam_proj.projection_type = CAMERA_PROJECTION_PERSPECTIVE;
		cam_proj.params.pers.fovy = 80.0f * TO_RADIAN;
		cam_proj.params.pers.aspect = win_aspect_ratio;
		cam_proj.params.pers.n = 1.0f;
		cam_proj.params.pers.f = 1000.0f;

		ProjectionMatrix = glm::perspective(cam_proj.params.pers.fovy, cam_proj.params.pers.aspect,
			cam_proj.params.pers.n, cam_proj.params.pers.f);
		// 뷰포트 - 화면에 도시
		view_port.x = 10;
		view_port.y = win_height - 10*2 - 150*2;
		view_port.w = 200;
		view_port.h = 150;
		break;
	}
	case CAMERA_CC_2: {
		flag_valid = true;
		flag_move = false; // yes. the main camera is permitted to move

		// let's use glm funtions to set up the initial camera pose
		cam_view.pos = glm::vec3(180.0f, 130.0f, 45.0f);
		glm::vec3 target = glm::vec3(170.0f, 120.0f, 10.0f);
		ViewMatrix = glm::lookAt(cam_view.pos, target, glm::vec3(0.0f, 0.0f, 1.0f)); // initial pose for main camera
		// 카메라의 위치, 카메라가 세상을 바라보는 방향, 영상의 위쪽 방향
		cam_view.uaxis = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		cam_view.vaxis = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		cam_view.naxis = glm::vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);

		cam_proj.projection_type = CAMERA_PROJECTION_PERSPECTIVE;
		cam_proj.params.pers.fovy = 70.0f * TO_RADIAN;
		cam_proj.params.pers.aspect = win_aspect_ratio;
		cam_proj.params.pers.n = 1.0f;
		cam_proj.params.pers.f = 1000.0f;

		ProjectionMatrix = glm::perspective(cam_proj.params.pers.fovy, cam_proj.params.pers.aspect,
			cam_proj.params.pers.n, cam_proj.params.pers.f);
		// 뷰포트 - 화면에 도시
		view_port.x = 10;
		view_port.y = win_height - 10*3 - 150*3;
		view_port.w = 200;
		view_port.h = 150;
		break;
	}
	case CAMERA_CC_DYNAMIC: {
		flag_valid = true;
		flag_move = true;

		glm::vec3 cctv_position = glm::vec3(100.0f, 75.0f, 45.0f);  // 건물 중앙 천장
		glm::vec3 cctv_target = glm::vec3(80.0f, 60.0f, 10.0f);     // 거실 바닥 쪽

		ViewMatrix = glm::lookAt(cctv_position, cctv_target, glm::vec3(0.0f, 0.0f, 1.0f));
		// 카메라의 위치, 카메라가 세상을 바라보는 방향, 영상의 위쪽 방향

		cam_view.uaxis = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		cam_view.vaxis = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		cam_view.naxis = glm::vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);
		// 카메라 속성들이 계속해서 변화하는 동적 cam이므로
		// 역계산을 통한 pos 다시 계산해야 함
		R33_t = glm::transpose(glm::mat3(ViewMatrix));
		T = glm::mat4(R33_t) * ViewMatrix;
		cam_view.pos = -glm::vec3(T[3][0], T[3][1], T[3][2]); // why does this work?

		cam_proj.projection_type = CAMERA_PROJECTION_PERSPECTIVE;
		cam_proj.params.pers.fovy = 60.0f * TO_RADIAN;
		cam_proj.params.pers.aspect = win_aspect_ratio;
		cam_proj.params.pers.n = 1.0f;
		cam_proj.params.pers.f = 50000.0f;

		ProjectionMatrix = glm::perspective(cam_proj.params.pers.fovy, cam_proj.params.pers.aspect,
			cam_proj.params.pers.n, cam_proj.params.pers.f);
		
		view_port.x = 10;
		view_port.y = win_height - 10*4 - 150*4;
		view_port.w = 200;
		view_port.h = 150;;
		break;
	}
	}
}

void Orthographic_Camera::define_camera(int win_width, int win_height, float win_aspect_ratio) {
	switch (camera_id) {
	case CAMERA_SIDE_FRONT: {// 정면도
		flag_valid = true;
		flag_move = false;

		glm::vec3 eye_pos = glm::vec3(115.0f, -300.0f, 25.0f); // 정면에서
		glm::vec3 target = glm::vec3(115.0f, 50.0f, 25.0f); // 건물 중심 바라보기

		ViewMatrix = glm::lookAt(eye_pos, target, glm::vec3(0.0f, 0.0f, 1.0f));
		// 카메라의 위치, 카메라가 세상을 바라보는 방향, 영상의 위쪽 방향

		cam_view.uaxis = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		cam_view.vaxis = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		cam_view.naxis = glm::vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);


		cam_proj.projection_type = CAMERA_PROJECTION_ORTHOGRAPHIC;

		float building_width = 230.0f;  
		float building_height = 50.0f;   
		cam_proj.params.ortho.left = -building_width / 2;
		cam_proj.params.ortho.right = building_width / 2 + 10;
		cam_proj.params.ortho.bottom = - building_height / 2 - 10;
		cam_proj.params.ortho.top = building_height / 2 + 10;
		cam_proj.params.ortho.n = 1.0f;
		cam_proj.params.ortho.f = 500.0f;

		ProjectionMatrix = glm::ortho(cam_proj.params.ortho.left, cam_proj.params.ortho.right,
			cam_proj.params.ortho.bottom, cam_proj.params.ortho.top,
			cam_proj.params.ortho.n, cam_proj.params.ortho.f);

		view_port.x = win_width - 400;
		view_port.y = win_height - 10 - 100;
		view_port.w = 400;
		view_port.h = 100;
		break;
	}
	
	case CAMERA_TOP: {// 상면도
		flag_valid = true;
		flag_move = false;

		glm::vec3 eye_pos = glm::vec3(115.0f, 80.0f, 200.0f);
		glm::vec3 target = glm::vec3(115.0f, 80.0f, 0.0f);

		ViewMatrix = glm::lookAt(eye_pos, target, glm::vec3(0.0f, 1.0f, 0.0f));
		// 카메라의 위치, 카메라가 세상을 바라보는 방향, 영상의 위쪽 방향

		cam_view.uaxis = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		cam_view.vaxis = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		cam_view.naxis = glm::vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);


		cam_proj.projection_type = CAMERA_PROJECTION_ORTHOGRAPHIC;

		float building_width = 230.0f;
		float building_depth = 160.0f;
		cam_proj.params.ortho.left = -building_width / 2;
		cam_proj.params.ortho.right = building_width / 2 + 10;
		cam_proj.params.ortho.bottom = -building_depth / 2;
		cam_proj.params.ortho.top = building_depth / 2 + 10;
		cam_proj.params.ortho.n = 1.0f;
		cam_proj.params.ortho.f = 500.0f;

		ProjectionMatrix = glm::ortho(cam_proj.params.ortho.left, cam_proj.params.ortho.right,
			cam_proj.params.ortho.bottom, cam_proj.params.ortho.top,
			cam_proj.params.ortho.n, cam_proj.params.ortho.f);

		view_port.x = win_width - 400;
		view_port.y = win_height - 230*2;
		view_port.w = 380;
		view_port.h = 230;
		break;
	}

	case CAMERA_SIDE: {// 측면도
		flag_valid = true;
		flag_move = false;

		glm::vec3 eye_pos = glm::vec3(-200.0f, 80.0f, 25.0f);
		glm::vec3 target = glm::vec3(200.0f, 80.0f, 25.0f);

		ViewMatrix = glm::lookAt(eye_pos, target, glm::vec3(0.0f, 0.0f, 1.0f));
		// 카메라의 위치, 카메라가 세상을 바라보는 방향, 영상의 위쪽 방향

		cam_view.uaxis = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		cam_view.vaxis = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		cam_view.naxis = glm::vec3(ViewMatrix[0][2], ViewMatrix[1][2], ViewMatrix[2][2]);


		cam_proj.projection_type = CAMERA_PROJECTION_ORTHOGRAPHIC;

		float building_depth = 160.0f;
		float building_height = 50.0f;
		cam_proj.params.ortho.left = -building_depth / 2;
		cam_proj.params.ortho.right = building_depth / 2;
		cam_proj.params.ortho.bottom = -building_height / 2 - 10;
		cam_proj.params.ortho.top = building_height / 2 + 10;
		cam_proj.params.ortho.n = 1.0f;
		cam_proj.params.ortho.f = 500.0f;

		ProjectionMatrix = glm::ortho(cam_proj.params.ortho.left, cam_proj.params.ortho.right,
			cam_proj.params.ortho.bottom, cam_proj.params.ortho.top,
			cam_proj.params.ortho.n, cam_proj.params.ortho.f);

		view_port.x = win_width - 400;
		view_port.y = win_height - 10 - 230*3;
		view_port.w = 320;
		view_port.h = 100;
		break;
	}
	}
}
