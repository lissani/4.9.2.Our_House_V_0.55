#define _CRT_SECURE_NO_WARNINGS

#include "Scene_Definitions.h"
enum class PathType {
    Wolf,
    Spider
};

// ��� ����Ʈ
struct PathPoint {
    glm::vec3 position;
    float rotation;
    bool is_turn_point;
    float curve_radius;
};

struct CurveSegment {
    glm::vec3 start_pos;
    glm::vec3 end_pos;
    glm::vec3 center;     // ��ȣ�� �߽���
    float start_angle;    // ���� ����
    float end_angle;      // �� ����
    float radius;         // ������
    bool is_curve;        // ����� ��������
    bool is_valid;        // ��ȿ�� �����
};

class PathFollower {
private:
    PathType path_type;
    std::vector<PathPoint> path_points;
    std::vector<CurveSegment> curve_segments;
    int current_segment;
    float movement_speed;
    glm::vec3 current_position;
    float current_rotation;
    bool loop_path;
    float segment_progress;

    bool ping_pong_mode;     // ���� ��� Ȱ��ȭ
    bool moving_forward;     // ������ �̵� ������
    std::vector<CurveSegment> reverse_segments; // ������ ���׸�Ʈ��

public:
    PathFollower(PathType type) : path_type(type), current_segment(0), movement_speed(0.075f),
        current_rotation(0.0f), loop_path(true), segment_progress(0.0f),
        ping_pong_mode(true), moving_forward(true) {  // ���� ��� �⺻ Ȱ��ȭ
    }

    void setup_path() {
        switch (path_type) {
        case PathType::Wolf:
            setup_path_wolf();
            break;
        case PathType::Spider:
            setup_path_spider();
            break;
        }
    }

    void setup_path_spider() {
        path_points.clear();
        curve_segments.clear();

        auto offset = glm::vec3(0.0f, 0.0f, 0.0f);
        path_points.push_back({ glm::vec3(185.0f, 27.0f, 0.0f) + offset, 0.0f, false, 0.0f });
        path_points.push_back({ glm::vec3(185.0f, 40.0f, 0.0f) + offset, 0.0f, false, 0.0f });
        path_points.push_back({ glm::vec3(185.0f, 55.0f, 0.0f) + offset, 0.0f, true, 25.0f });
        path_points.push_back({ glm::vec3(210.0f, 65.0f, 0.0f) + offset, 0.0f, false, 0.0f });
        path_points.push_back({ glm::vec3(210.0f, 85.0f, 0.0f) + offset, 0.0f, true, 10.0f });
        path_points.push_back({ glm::vec3(195.0f, 100.0f, 0.0f) + offset, 0.0f, false, 0.0f });
        path_points.push_back({ glm::vec3(180.0f, 100.0f, 0.0f) + offset, 0.0f, true, 10.0f });
        path_points.push_back({ glm::vec3(165.0f, 115.0f, 0.0f) + offset, 0.0f, false, 0.0f });
        path_points.push_back({ glm::vec3(165.0f, 135.0f, 0.0f) + offset, 0.0f, true, 10.0f });
        path_points.push_back({ glm::vec3(150.0f, 148.0f, 0.0f) + offset, 0.0f, false, 0.0f });

        calculate_curve_segments();
        calculate_reverse_segments(); // ������ ���׸�Ʈ ���

        current_segment = 0;
        current_position = path_points[0].position;
        current_rotation = path_points[0].rotation;
        segment_progress = 0.0f;
        moving_forward = true;

        printf("PathFollower initialized with %d forward and %d reverse segments\n",
            (int)curve_segments.size(), (int)reverse_segments.size());
    }

    void setup_path_wolf() {
        path_points.clear();
        curve_segments.clear();

        auto offset = glm::vec3(0.0f, 0.0f, 0.0f);
        path_points.push_back({ glm::vec3(40.0f, 85.0f, 0.0f) + offset, 0.0f, false, 0.0f });
        path_points.push_back({ glm::vec3(110.0f, 85.0f, 0.0f) + offset, 0.0f, false, 0.0f });
     
        calculate_curve_segments();
        calculate_reverse_segments(); // ������ ���׸�Ʈ ���

        current_segment = 0;
        current_position = path_points[0].position;
        current_rotation = path_points[0].rotation;
        segment_progress = 0.0f;
        moving_forward = true;

        printf("PathFollower initialized with %d forward and %d reverse segments\n",
            (int)curve_segments.size(), (int)reverse_segments.size());
    }

    void calculate_reverse_segments() {
        reverse_segments.clear();

        // ������ ���׸�Ʈ�� �������� ������
        for (int i = curve_segments.size() - 1; i >= 0; i--) {
            CurveSegment reverse_segment = curve_segments[i];

            // �������� ���� ������ ��ü
            glm::vec3 temp_pos = reverse_segment.start_pos;
            reverse_segment.start_pos = reverse_segment.end_pos;
            reverse_segment.end_pos = temp_pos;

            if (reverse_segment.is_curve && reverse_segment.is_valid) {
                // ��� ��� ������ ������ ������
                float temp_angle = reverse_segment.start_angle;
                reverse_segment.start_angle = reverse_segment.end_angle;
                reverse_segment.end_angle = temp_angle;

                // �߿�: ���� ���⵵ �������Ѿ� ��
                if (reverse_segment.start_angle > reverse_segment.end_angle) {
                    if (reverse_segment.start_angle - reverse_segment.end_angle > glm::pi<float>()) {
                        reverse_segment.end_angle += 2.0f * glm::pi<float>();
                    }
                }
                else {
                    if (reverse_segment.end_angle - reverse_segment.start_angle > glm::pi<float>()) {
                        reverse_segment.start_angle += 2.0f * glm::pi<float>();
                    }
                }
            }

            reverse_segments.push_back(reverse_segment);
        }

        printf("Created %d reverse segments\n", (int)reverse_segments.size());

        if (!curve_segments.empty() && !reverse_segments.empty()) {
            printf("Forward end: (%.1f, %.1f) -> Reverse start: (%.1f, %.1f)\n",
                curve_segments.back().end_pos.x, curve_segments.back().end_pos.y,
                reverse_segments[0].start_pos.x, reverse_segments[0].start_pos.y);
            printf("Reverse end: (%.1f, %.1f) -> Forward start: (%.1f, %.1f)\n",
                reverse_segments.back().end_pos.x, reverse_segments.back().end_pos.y,
                curve_segments[0].start_pos.x, curve_segments[0].start_pos.y);
        }
    }

    void calculate_curve_segments() {
        for (int i = 0; i < path_points.size() - 1; i++) {
            CurveSegment segment;
            segment.is_valid = false;

            PathPoint& current = path_points[i];
            PathPoint& next = path_points[i + 1];

            segment.start_pos = current.position;
            segment.end_pos = next.position;

            if (next.is_turn_point && next.curve_radius > 0.0f && i + 2 < path_points.size()) {
                glm::vec3 P1 = current.position;
                glm::vec3 P2 = next.position;
                glm::vec3 P3 = path_points[i + 2].position;

                if (create_smooth_curve(P1, P2, P3, next.curve_radius, segment)) {
                    segment.is_curve = true;
                    segment.is_valid = true;
                    segment.start_pos = P1;
                    segment.end_pos = P2;
                }
                else {
                    segment.is_curve = false;
                }
            }
            else {
                segment.is_curve = false;
                segment.is_valid = true;
            }

            curve_segments.push_back(segment);
        }
    }

    void update_movement(int time_stamp) {
        std::vector<CurveSegment>& active_segments = moving_forward ? curve_segments : reverse_segments;

        if (active_segments.empty()) return;

        // ��� �Ϸ� Ȯ��
        if (current_segment >= active_segments.size()) {
            if (ping_pong_mode) {
                // ���� ���: ���� ��ȯ
                moving_forward = !moving_forward;
                current_segment = 0;
                segment_progress = 0.0f;

                // ���ο� ������ ���׸�Ʈ �迭 ������Ʈ
                active_segments = moving_forward ? curve_segments : reverse_segments;

                // ���Ӽ� ����: ���� ��ġ�� �״�� ����
                // ���⸸ �ٲ�� ��ġ�� ���������� �̾���

                printf("Direction changed: %s (pos: %.1f, %.1f)\n",
                    moving_forward ? "Forward" : "Backward",
                    current_position.x, current_position.y);
            }
            else if (loop_path) {
                // ���� ���� ���
                current_segment = 0;
                current_position = path_points[0].position;
                current_rotation = path_points[0].rotation;
                segment_progress = 0.0f;
            }
            else {
                return;
            }
            return; // ���� ��ȯ �� �̹� �������� �ǳʶٱ�
        }

        CurveSegment& segment = active_segments[current_segment];

        if (segment.is_curve && segment.is_valid) {
            move_along_curve(segment);
        }
        else {
            move_along_line(segment);
        }

        // ���׸�Ʈ �Ϸ� Ȯ��
        if (segment_progress >= 1.0f) {
            current_position = segment.end_pos;
            current_segment++;
            segment_progress = 0.0f;
        }
    }

    void move_along_curve(CurveSegment& segment) {
        glm::vec3 to_start = segment.start_pos - segment.center;
        glm::vec3 to_end = segment.end_pos - segment.center;

        float start_angle = atan2(to_start.y, to_start.x);
        float end_angle = atan2(to_end.y, to_end.x);

        float PI = 180.0f * TO_RADIAN;
        float angle_diff = end_angle - start_angle;
        if (angle_diff > PI) end_angle -= 2 * PI;
        else if (angle_diff < -PI) end_angle += 2 * PI;

        float total_angle = end_angle - start_angle;
        float current_angle = start_angle + total_angle * segment_progress;

        float radius = glm::length(to_start);
        glm::vec3 new_position;
        new_position.x = segment.center.x + radius * cos(current_angle);
        new_position.y = segment.center.y + radius * sin(current_angle);
        new_position.z = segment.start_pos.z;

        float tangent_angle = current_angle + (total_angle > 0 ? 1.5708f : -1.5708f);
        current_rotation = tangent_angle;

        current_position = new_position;

        float arc_length = abs(total_angle) * radius;
        if (arc_length > 0.1f) {
            segment_progress += movement_speed / arc_length;
        }
        else {
            segment_progress += 0.1f;
        }

        segment_progress = glm::min(segment_progress, 1.0f);
    }

    void move_along_line(CurveSegment& segment) {
        glm::vec3 direction = segment.end_pos - segment.start_pos;
        float distance = glm::length(direction);

        if (distance > 0.001f) {
            direction = glm::normalize(direction);
            current_rotation = atan2(direction.y, direction.x);
            current_position = segment.start_pos + direction * (distance * segment_progress);
            segment_progress += movement_speed / distance;
            segment_progress = glm::min(segment_progress, 1.0f);
        }
        else {
            segment_progress = 1.0f;
        }
    }

    // ��� ���� �Լ��� (���� �߰�)
    void set_ping_pong_mode(bool enable) {
        ping_pong_mode = enable;
        if (!enable) {
            moving_forward = true; // ���� ��� ��� ���������� ����
        }
    }

    bool is_ping_pong_mode() const {
        return ping_pong_mode;
    }

    bool is_moving_forward() const {
        return moving_forward;
    }

    glm::mat4 get_transform_matrix() {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, current_position);
        transform = glm::rotate(transform, current_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        return transform;
    }

    glm::vec3 get_current_position() const {
        return current_position;
    }

    bool create_smooth_curve(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, float desired_radius, CurveSegment& segment) {
        glm::vec3 v1 = glm::normalize(P2 - P1);
        glm::vec3 v2 = glm::normalize(P3 - P2);

        float dot_product = glm::dot(v1, v2);
        if (abs(dot_product) > 0.99f) {
            return false;
        }

        float angle_between = acos(glm::clamp(dot_product, -1.0f, 1.0f));
        float min_radius = glm::length(P2 - P1) * 0.2f;
        float actual_radius = glm::max(desired_radius, min_radius);

        glm::vec3 bisector = glm::normalize(v1 + v2);
        float center_distance = actual_radius / sin(angle_between * 0.5f);

        float cross_z = v1.x * v2.y - v1.y * v2.x;
        glm::vec3 center_dir = bisector;
        if (cross_z > 0) {
            center_dir = glm::vec3(-bisector.y, bisector.x, 0.0f);
        }
        else {
            center_dir = glm::vec3(bisector.y, -bisector.x, 0.0f);
        }

        segment.center = P2 + center_dir * center_distance;
        segment.radius = actual_radius;

        glm::vec3 to_start = P1 - segment.center;
        glm::vec3 to_end = P3 - segment.center;

        segment.start_angle = atan2(to_start.y, to_start.x);
        segment.end_angle = atan2(to_end.y, to_end.x);

        segment.start_pos = segment.center + glm::normalize(to_start) * actual_radius;
        segment.end_pos = segment.center + glm::normalize(to_end) * actual_radius;

        float PI = 180.0f * TO_RADIAN;
        float angle_diff = segment.end_angle - segment.start_angle;

        if (angle_diff > PI) segment.end_angle -= 2 * PI;
        else if (angle_diff < -PI) segment.end_angle += 2 * PI;

        return true;
    }
};

class PathFollowingWolf {
private:
    PathFollower path_follower;

public:
    PathFollowingWolf() : path_follower(PathType::Wolf) {}
    void initialize() {
        path_follower.setup_path();
    }

    glm::mat4 get_movement_matrix(int time_stamp) {
        path_follower.update_movement(time_stamp);
        return path_follower.get_transform_matrix();
    }
};

class PathFollowingSpider {
private:
    PathFollower path_follower;

public:
    PathFollowingSpider() : path_follower(PathType::Spider) {}
    void initialize() {
        path_follower.setup_path();
    }

    glm::mat4 get_movement_matrix(int time_stamp) {
        path_follower.update_movement(time_stamp);
        return path_follower.get_transform_matrix();
    }
};

PathFollowingWolf path_following_wolf;
PathFollowingSpider path_following_spider;

///////////////
void Wolf_D::define_object() {
#define N_WOLF_FRAMES 17
    glm::mat4* cur_MM;
    Material* cur_material;
    flag_valid = true;

    for (int i = 0; i < N_WOLF_FRAMES; i++) {
        object_frames.emplace_back();
        sprintf(object_frames[i].filename, "Data/dynamic_objects/wolf/wolf_%02d_vnt.geom", i);
        object_frames[i].n_fields = 8;
        object_frames[i].front_face_mode = GL_CW;
        object_frames[i].prepare_geom_of_static_object();

        object_frames[i].instances.emplace_back();
        cur_MM = &(object_frames[i].instances.back().ModelMatrix);

        *cur_MM = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        *cur_MM = glm::scale(*cur_MM, glm::vec3(25.0f, 25.0f, 25.0f));
        *cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
        *cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

        cur_material = &(object_frames[i].instances.back().material);
        cur_material->emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        cur_material->ambient = glm::vec4(0.329412f, 0.223529f, 0.027451f, 1.0f);
        cur_material->diffuse = glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f);
        cur_material->specular = glm::vec4(0.992157f, 0.941176f, 0.807843f, 1.0f);
        cur_material->exponent = 128.0f * 0.21794872f;
    }
    path_following_wolf.initialize();
}

void Spider_D::define_object() {
#define N_SPIDER_FRAMES 16
    glm::mat4* cur_MM;
    Material* cur_material;
    flag_valid = true;

    for (int i = 0; i < N_SPIDER_FRAMES; i++) {
        object_frames.emplace_back();
        sprintf(object_frames[i].filename, "Data/dynamic_objects/spider/spider_vnt_%02d.geom", i);
        object_frames[i].n_fields = 8;
        object_frames[i].front_face_mode = GL_CW;
        object_frames[i].prepare_geom_of_static_object();

        object_frames[i].instances.emplace_back();
        cur_MM = &(object_frames[i].instances.back().ModelMatrix);

        *cur_MM = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        *cur_MM = glm::scale(*cur_MM, glm::vec3(3.0f, 3.0f, 3.0f));
        *cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, -1.0f));
        *cur_MM = glm::rotate(*cur_MM, 90.0f * TO_RADIAN, glm::vec3(-1.0f, 0.0f, 0.0f));

        cur_material = &(object_frames[i].instances.back().material);
        cur_material->emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        cur_material->ambient = glm::vec4(0.329412f, 0.223529f, 0.027451f, 1.0f);
        cur_material->diffuse = glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f);
        cur_material->specular = glm::vec4(0.992157f, 0.941176f, 0.807843f, 1.0f);
        cur_material->exponent = 128.0f * 0.21794872f;
    }
    path_following_spider.initialize();
}

void Dynamic_Object::draw_object(glm::mat4& ViewMatrix, glm::mat4& ProjectionMatrix, SHADER_ID shader_kind,
    std::vector<std::reference_wrapper<Shader>>& shader_list, int time_stamp) {
    int cur_object_index = time_stamp % object_frames.size();
    Static_Object& cur_object = object_frames[cur_object_index];
    glFrontFace(cur_object.front_face_mode);

    float rotation_angle = 0.0f;
    glm::mat4 ModelMatrix = glm::mat4(1.0f);

    switch (object_id) {
    case DYNAMIC_OBJECT_WOLF:
        ModelMatrix = path_following_wolf.get_movement_matrix(time_stamp);
        break;
    case DYNAMIC_OBJECT_SPIDER:
        ModelMatrix = path_following_spider.get_movement_matrix(time_stamp);
        break;
    }

    for (int i = 0; i < cur_object.instances.size(); i++) {
        glm::mat4 ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix * cur_object.instances[i].ModelMatrix;
        switch (shader_kind) {
        case SHADER_SIMPLE: {
            Shader_Simple* shader_simple_ptr = static_cast<Shader_Simple*>(&shader_list[shader_ID_mapper[shader_kind]].get());
            glUseProgram(shader_simple_ptr->h_ShaderProgram);
            glUniformMatrix4fv(shader_simple_ptr->loc_ModelViewProjectionMatrix, 1, GL_FALSE,
                &ModelViewProjectionMatrix[0][0]);
            glUniform3f(shader_simple_ptr->loc_primitive_color, cur_object.instances[i].material.diffuse.r,
                cur_object.instances[i].material.diffuse.g, cur_object.instances[i].material.diffuse.b);
            break;
        }
        case SHADER_GOURAUD: {
            auto* shader = static_cast<Shader_Gouraud*>(&shader_list[shader_ID_mapper[shader_kind]].get());
            glUseProgram(shader->h_ShaderProgram);

            glm::mat4 FinalModelMatrix = ModelMatrix * cur_object.instances[i].ModelMatrix;
            glm::mat4 MV = ViewMatrix * FinalModelMatrix;
            glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));
            glm::mat4 MVP = ProjectionMatrix * MV;

            // 라이트 위치 (월드 좌표계)
            glm::vec3 light_position_world = glm::vec3(0.0f, 0.0f, 100.0f);
            glm::vec3 light_position_view = glm::vec3(ViewMatrix * glm::vec4(light_position_world, 1.0f));
            glm::vec3 view_position_view = glm::vec3(0.0f);

            // 유니폼 변수 설정
            glUniformMatrix4fv(shader->loc_ModelViewMatrix, 1, GL_FALSE, &MV[0][0]);
            glUniformMatrix4fv(shader->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix3fv(shader->loc_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);

            glUniform3fv(shader->loc_light_position, 1, &light_position_view[0]);
            glUniform3fv(shader->loc_view_position, 1, &view_position_view[0]);

            // 머티리얼 속성 설정
            Material& m = cur_object.instances[i].material;
            glUniform4fv(shader->loc_material_ambient, 1, &m.ambient[0]);
            glUniform4fv(shader->loc_material_diffuse, 1, &m.diffuse[0]);
            glUniform4fv(shader->loc_material_specular, 1, &m.specular[0]);
            glUniform1f(shader->loc_material_shininess, m.exponent);
            break;
        }
        case SHADER_PHONG: {
            auto* shader = static_cast<Shader_Phong*>(&shader_list[shader_ID_mapper[shader_kind]].get());
            glUseProgram(shader->h_ShaderProgram);

            glm::mat4 FinalModelMatrix = ModelMatrix * cur_object.instances[i].ModelMatrix;
            glm::mat4 MV = ViewMatrix * FinalModelMatrix;
            glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));
            glm::mat4 MVP = ProjectionMatrix * MV;

            // 라이트 위치 (월드 좌표계)
            glm::vec3 light_position_world = glm::vec3(0.0f, 0.0f, 100.0f);
            glm::vec3 light_position_view = glm::vec3(ViewMatrix * glm::vec4(light_position_world, 1.0f));
            glm::vec3 view_position_view = glm::vec3(0.0f);

            // 유니폼 변수 설정
            glUniformMatrix4fv(shader->loc_ModelViewMatrix, 1, GL_FALSE, &MV[0][0]);
            glUniformMatrix4fv(shader->loc_ModelViewProjectionMatrix, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix3fv(shader->loc_NormalMatrix, 1, GL_FALSE, &NormalMatrix[0][0]);

            glUniform3fv(shader->loc_light_position, 1, &light_position_view[0]);
            glUniform3fv(shader->loc_view_position, 1, &view_position_view[0]);

            // 머티리얼 속성 설정
            Material& m = cur_object.instances[i].material;
            glUniform4fv(shader->loc_material_ambient, 1, &m.ambient[0]);
            glUniform4fv(shader->loc_material_diffuse, 1, &m.diffuse[0]);
            glUniform4fv(shader->loc_material_specular, 1, &m.specular[0]);
            glUniform1f(shader->loc_material_shininess, m.exponent);
            break;
        }
        }
        glBindVertexArray(cur_object.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3 * cur_object.n_triangles);
        glBindVertexArray(0);
        glUseProgram(0);
    }
}