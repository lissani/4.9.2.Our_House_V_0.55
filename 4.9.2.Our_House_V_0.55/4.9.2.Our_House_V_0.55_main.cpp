#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Shaders/LoadShaders.h"
#include "Scene_Definitions.h"

Scene scene;
int main_camera_index = 0;
static int original_main_camera_index = 0;
static bool cctv_control_mode = false;

void move_main_camera(int axis, float amount) {
	if (main_camera_index == 4) { // cctv ��忡���� ��� ����
		return;
	}
	Camera& cam = scene.camera_list[main_camera_index].get();

	glm::vec3 delta(0.0f);
	if (axis == 0) {        // U�� (ī�޶� ������)
		delta = cam.cam_view.uaxis * amount;
	}
	else if (axis == 1) {   // V�� (ī�޶� ����)
		delta = cam.cam_view.vaxis * amount;
	}
	else if (axis == 2) {   // N�� (ī�޶� ����, ������ ������ ����)
		delta = cam.cam_view.naxis * amount;
	}
	cam.cam_view.pos += delta;
	cam.ViewMatrix = glm::lookAt(cam.cam_view.pos, cam.cam_view.pos - cam.cam_view.naxis, cam.cam_view.vaxis);
}

void rotate_main_camera(int axis, float angle) {
	Camera& cam = scene.camera_list[main_camera_index].get();
	glm::mat4 R = glm::mat4(1.0f);
	if (axis == 0) R = glm::rotate(R, angle, cam.cam_view.uaxis);
	if (axis == 1) R = glm::rotate(R, angle, cam.cam_view.vaxis);
	if (axis == 2) R = glm::rotate(R, angle, cam.cam_view.naxis);

	cam.cam_view.uaxis = glm::mat3(R) * cam.cam_view.uaxis;
	cam.cam_view.vaxis = glm::mat3(R) * cam.cam_view.vaxis;
	cam.cam_view.naxis = glm::mat3(R) * cam.cam_view.naxis;

	cam.ViewMatrix = glm::lookAt(cam.cam_view.pos, cam.cam_view.pos - cam.cam_view.naxis, cam.cam_view.vaxis);
}

void zoom_main_camera(float zoom_factor) {
	Camera& cam = scene.camera_list[main_camera_index].get();
	float& fovy = cam.cam_proj.params.pers.fovy;
	
	// fov ���� ����
	if (main_camera_index == 4) {  // CCTV
		fovy = glm::clamp(fovy * zoom_factor, 15.0f * TO_RADIAN, 90.0f * TO_RADIAN);
	}
	else {  // �Ϲ� ī�޶�
		fovy = glm::clamp(fovy * zoom_factor, 10.0f * TO_RADIAN, 120.0f * TO_RADIAN);
	}

	cam.ProjectionMatrix = glm::perspective(cam.cam_proj.params.pers.fovy, cam.cam_proj.params.pers.aspect,
		cam.cam_proj.params.pers.n, cam.cam_proj.params.pers.f);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto camera = scene.camera_list.begin(); camera != scene.camera_list.end(); camera++) {
		if (camera->get().flag_valid == false) continue;
		glViewport(camera->get().view_port.x, camera->get().view_port.y,
			camera->get().view_port.w, camera->get().view_port.h);
		scene.ViewMatrix = camera->get().ViewMatrix;
		scene.ProjectionMatrix = camera->get().ProjectionMatrix;

		scene.draw_world();
	}
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	static int flag_cull_face = 0, polygon_fill_on = 0, depth_test_on = 0;
	static bool rotation_mode = false;

	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	case 'c':
		flag_cull_face = (flag_cull_face + 1) % 3;
		switch (flag_cull_face) {
		case 0:
			glDisable(GL_CULL_FACE);
			glutPostRedisplay();
			fprintf(stdout, "^^^ No faces are culled.\n");
			break;
		case 1: // cull back faces;
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			fprintf(stdout, "^^^ Back faces are culled.\n");
			break;
		case 2: // cull front faces;
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			fprintf(stdout, "^^^ Front faces are culled.\n");
			break;
		}
		break;
	case 'f':
		polygon_fill_on = 1 - polygon_fill_on;
		if (polygon_fill_on) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			fprintf(stdout, "^^^ Polygon filling enabled.\n");
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			fprintf(stdout, "^^^ Line drawing enabled.\n");
		}
		glutPostRedisplay();
		break;
	case 'd':
		depth_test_on = 1 - depth_test_on;
		if (depth_test_on) {
			glEnable(GL_DEPTH_TEST);
			fprintf(stdout, "^^^ Depth test enabled.\n");
		}
		else {
			glDisable(GL_DEPTH_TEST);
			fprintf(stdout, "^^^ Depth test disabled.\n");
		}
		glutPostRedisplay();
		break;

	case 'v':
		cctv_control_mode = !cctv_control_mode;
		if (cctv_control_mode) {
			original_main_camera_index = main_camera_index; // ���� ī�޶� ����
			main_camera_index = 4;
			printf("cctv control mode on - rotate/zoom only\n");
		}
		else {
			main_camera_index = original_main_camera_index;
			printf("back to main cam control");
		}
		break;
	case ' ':
		rotation_mode = !rotation_mode;
		printf("mode: %s\n", rotation_mode ? "rotation" : "movement");
		break;

		// ���� ������ keyborad �Է�
	case 'q':
		if (rotation_mode)
			rotate_main_camera(0, -1.0f * TO_RADIAN);
		else
			move_main_camera(0, -10.0f);
		break;

	case 'w':
		if (rotation_mode) rotate_main_camera(0, 1.0f * TO_RADIAN);
		else
			move_main_camera(0, +10.0f);
		break;

	case 'e':
		if (rotation_mode) rotate_main_camera(1, -1.0f * TO_RADIAN);
		else
			move_main_camera(1, -10.0f);
		break;

	case 'r':
		if (rotation_mode) rotate_main_camera(1, 1.0f * TO_RADIAN);
		else
			move_main_camera(1, +10.0f);
		break;

	case 't':
		if (rotation_mode) rotate_main_camera(2, -1.0f * TO_RADIAN);
		else
			move_main_camera(2, -10.0f);
		break;

	case 'y':
		if (rotation_mode) rotate_main_camera(2, 1.0f * TO_RADIAN);
		else
			move_main_camera(2, +10.0f);
		break;

	case 'x':
		scene.show_camera_frames = !scene.show_camera_frames;
		printf("Camera frame display: %s\n", scene.show_camera_frames ? "ON" : "OFF");
		glutPostRedisplay();
		break;

	case '1': 
		scene.shader_kind = SHADER_GOURAUD;
		printf("Gouraud shading\n");
		glutPostRedisplay();
		break;

	case '2': 
		scene.shader_kind = SHADER_PHONG;
		printf("Phong shading\n");
		glutPostRedisplay();
		break;

	case '0':  // Simple 쉐이더로 되돌리기 위한 키 추가
		scene.shader_kind = SHADER_SIMPLE;
		printf("Simple shading\n");
		glutPostRedisplay();
		break;
	}
	glutPostRedisplay();
}

void mouse_wheel(int wheel, int direction, int x, int y) {
	if (direction > 0)
		zoom_main_camera(0.95f);
	else
		zoom_main_camera(1.05f);
	glutPostRedisplay();
}

void reshape(int width, int height) {
	scene.window.width = width;
	scene.window.height = height;
	scene.window.aspect_ratio = (float)width / height;
	scene.create_camera_list(scene.window.width, scene.window.height, scene.window.aspect_ratio);
	glutPostRedisplay();
}

void timer_scene(int index) {
	scene.clock(0);
	glutPostRedisplay();
	glutTimerFunc(100, timer_scene, 0);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseWheelFunc(mouse_wheel);
	glutReshapeFunc(reshape);
 	glutTimerFunc(100, timer_scene, 0);
//	glutCloseFunc(cleanup_OpenGL_stuffs or else); // Do it yourself!!!
}

void initialize_OpenGL(void) {
	glEnable(GL_DEPTH_TEST); // Default state
	 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.12f, 0.18f, 0.12f, 1.0f);
}

void initialize_renderer(void) {
	register_callbacks();
	initialize_OpenGL();
	scene.initialize();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void print_message(const char * m) {
	fprintf(stdout, "%s\n\n", m);
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170/AIE4012 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) { 
	char program_name[256] = "Sogang CSE4170/AIE4120 Our_House_GLSL_V_0.55";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: fill it yourself!" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}
