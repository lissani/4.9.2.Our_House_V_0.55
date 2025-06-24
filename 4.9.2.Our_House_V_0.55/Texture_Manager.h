#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include <freeimage/FreeImage.h>

enum TEXTURE_FILTER_MODE {
    FILTER_NEAREST = 0,
    FILTER_LINEAR = 1
};

struct TextureInfo {
    GLuint texture_id;
    std::string filename;
    int width, height, channels;
    bool is_active;
};

class TextureManager {
private:
    std::vector<TextureInfo> textures;
    TEXTURE_FILTER_MODE current_filter_mode;

public:
    TextureManager() : current_filter_mode(FILTER_LINEAR) {}

    // 텍스처 로딩
    GLuint load_texture(const std::string& filename);

    // 필터 모드 변경
    void set_filter_mode(TEXTURE_FILTER_MODE mode);
    void toggle_filter_mode();

    // 현재 필터 모드 반환
    TEXTURE_FILTER_MODE get_filter_mode() const { return current_filter_mode; }

    // 모든 텍스처에 필터 적용
    void apply_filter_to_all_textures();

    // 텍스처 정리
    void cleanup();

    // 텍스처 정보 출력
    void print_texture_info();
};

extern TextureManager texture_manager;

#endif