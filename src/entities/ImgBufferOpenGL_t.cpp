#include "entities/ImgBufferOpenGL_t.hpp"
#include <algorithm>
#include <cmath>
#include <functional>

using APTracer::Entities::ImgBufferOpenGL_t;

ImgBufferOpenGL_t::ImgBufferOpenGL_t(size_t size_x, size_t size_y) : ImgBuffer_t(size_x, size_y), img_gl_(size_x_ * size_y_ * 3), tex_(0) {
#ifndef APTRACER_USE_OPENGL
    std::cerr << "Error: OpenGL image buffer created, but the library wasn't compiled with 'APTRACER_USE_OPENGL' defined. The image won't be shown on screen." << std::endl;
#endif
}

auto ImgBufferOpenGL_t::reset() -> void {
    updates_ = 0;
    std::fill(img_.begin(), img_.end(), Vec3f());
}

auto ImgBufferOpenGL_t::update(const std::vector<Vec3f>& img) -> void {
    ++updates_;

    std::transform(img_.begin(), img_.end(), img.begin(), img_.begin(), std::plus<>());
}

auto ImgBufferOpenGL_t::update() -> void {
    ++updates_;

#ifdef APTRACER_USE_OPENGL
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<GLsizei>(size_x_), static_cast<GLsizei>(size_y_), GL_RGB, GL_UNSIGNED_BYTE, img_gl_.data());

    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2i(-1, -1);
    glTexCoord2i(1, 0);
    glVertex2i(1, -1);
    glTexCoord2i(1, 1);
    glVertex2i(1, 1);
    glTexCoord2i(0, 1);
    glVertex2i(-1, 1);
    glEnd();

    glutSwapBuffers();
#endif
}

auto ImgBufferOpenGL_t::update(const Vec3f& colour, size_t pos_x, size_t pos_y) -> void {
    // img_[pos_y][pos_x] = img_[pos_y][pos_x] * (1.0 - 1.0/static_cast<double>(updates_)) + colour/static_cast<double>(updates_);
    constexpr double max_8_bits = 255.0;
    img_[pos_y * size_x_ + pos_x] += colour;
    img_gl_[((size_y_ - 1 - pos_y) * size_x_ + pos_x) * 3]     = static_cast<unsigned char>(std::lround(std::max(std::min(1.0, img_[pos_y * size_x_ + pos_x][0] / updates_), 0.0) * max_8_bits));
    img_gl_[((size_y_ - 1 - pos_y) * size_x_ + pos_x) * 3 + 1] = static_cast<unsigned char>(std::lround(std::max(std::min(1.0, img_[pos_y * size_x_ + pos_x][1] / updates_), 0.0) * max_8_bits));
    img_gl_[((size_y_ - 1 - pos_y) * size_x_ + pos_x) * 3 + 2] = static_cast<unsigned char>(std::lround(std::max(std::min(1.0, img_[pos_y * size_x_ + pos_x][2] / updates_), 0.0) * max_8_bits));
}

auto ImgBufferOpenGL_t::set(const std::vector<Vec3f>& img) -> void {
    updates_ = 1;
    img_     = img;
}

auto ImgBufferOpenGL_t::set(const Vec3f& colour, size_t pos_x, size_t pos_y) -> void {
    constexpr double max_8_bits                                = 255.0;
    img_[pos_y * size_x_ + pos_x]                              = colour;
    img_gl_[((size_y_ - 1 - pos_y) * size_x_ + pos_x) * 3]     = static_cast<unsigned char>(std::lround(std::max(std::min(1.0, img_[pos_y * size_x_ + pos_x][0] / updates_), 0.0) * max_8_bits));
    img_gl_[((size_y_ - 1 - pos_y) * size_x_ + pos_x) * 3 + 1] = static_cast<unsigned char>(std::lround(std::max(std::min(1.0, img_[pos_y * size_x_ + pos_x][1] / updates_), 0.0) * max_8_bits));
    img_gl_[((size_y_ - 1 - pos_y) * size_x_ + pos_x) * 3 + 2] = static_cast<unsigned char>(std::lround(std::max(std::min(1.0, img_[pos_y * size_x_ + pos_x][2] / updates_), 0.0) * max_8_bits));
}
