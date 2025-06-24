// Texture_Manager.cpp
#include "Texture_Manager.h"
#include <iostream>

TextureManager texture_manager; // 전역 인스턴스

// ==================== TextureManager 구현 ====================
GLuint TextureManager::load_texture(const std::string& filename) {
    // FreeImage를 사용한 텍스처 로딩
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(filename.c_str());
    }

    if (format == FIF_UNKNOWN) {
        std::cout << "Error: Cannot determine image format for " << filename << std::endl;
        return 0;
    }

    FIBITMAP* bitmap = FreeImage_Load(format, filename.c_str());
    if (!bitmap) {
        std::cout << "Error: Failed to load image " << filename << std::endl;
        return 0;
    }

    // 32비트로 변환 (RGBA)
    FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    int width = FreeImage_GetWidth(bitmap32);
    int height = FreeImage_GetHeight(bitmap32);
    BYTE* pixels = FreeImage_GetBits(bitmap32);

    // OpenGL 텍스처 생성
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // 텍스처 데이터 업로드 (FreeImage는 BGR 순서이므로 주의)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_BGRA, GL_UNSIGNED_BYTE, pixels);

    // 초기 필터 설정 (Linear가 기본값)
    if (current_filter_mode == FILTER_LINEAR) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    // 텍스처 래핑 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    // 텍스처 정보 저장
    TextureInfo tex_info;
    tex_info.texture_id = texture_id;
    tex_info.filename = filename;
    tex_info.width = width;
    tex_info.height = height;
    tex_info.channels = 4; // RGBA
    tex_info.is_active = true;

    textures.push_back(tex_info);

    FreeImage_Unload(bitmap32);

    std::cout << "Texture loaded: " << filename << " (" << width << "x" << height << ")" << std::endl;

    return texture_id;
}

void TextureManager::set_filter_mode(TEXTURE_FILTER_MODE mode) {
    current_filter_mode = mode;
    apply_filter_to_all_textures();

    std::cout << "Texture filter mode: " <<
        (mode == FILTER_LINEAR ? "LINEAR" : "NEAREST") << std::endl;
}

void TextureManager::toggle_filter_mode() {
    current_filter_mode = (current_filter_mode == FILTER_LINEAR) ?
        FILTER_NEAREST : FILTER_LINEAR;
    apply_filter_to_all_textures();

    std::cout << "Texture filter mode: " <<
        (current_filter_mode == FILTER_LINEAR ? "LINEAR" : "NEAREST") << std::endl;
}

void TextureManager::apply_filter_to_all_textures() {
    for (auto& tex_info : textures) {
        if (tex_info.is_active) {
            glBindTexture(GL_TEXTURE_2D, tex_info.texture_id);

            if (current_filter_mode == FILTER_LINEAR) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureManager::cleanup() {
    for (auto& tex_info : textures) {
        if (tex_info.is_active) {
            glDeleteTextures(1, &tex_info.texture_id);
        }
    }
    textures.clear();
}

void TextureManager::print_texture_info() {
    std::cout << "=== Texture Manager Info ===" << std::endl;
    std::cout << "Current filter mode: " <<
        (current_filter_mode == FILTER_LINEAR ? "LINEAR" : "NEAREST") << std::endl;
    std::cout << "Active textures: " << textures.size() << std::endl;

    for (size_t i = 0; i < textures.size(); i++) {
        const auto& tex = textures[i];
        std::cout << "  [" << i << "] " << tex.filename <<
            " (" << tex.width << "x" << tex.height << ") ID:" << tex.texture_id << std::endl;
    }
}