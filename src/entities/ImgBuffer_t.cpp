#include "entities/ImgBuffer_t.h"
#define cimg_use_jpeg
#define cimg_use_png
#define cimg_use_tiff
//#define cimg_use_tinyexr // Can't put this twice for some reason, is on in Texture_t.cpp
#define cimg_use_openmp 1
#define cimg_use_cpp11 1
#define cimg_display 0
#include "functions/CImg.h"
#include <cmath>
#include <algorithm>

using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Vec3f;

ImgBuffer_t::ImgBuffer_t(size_t size_x, size_t size_y): size_x_(size_x), size_y_(size_y), updates_(0) {
    img_ = new Vec3f[size_y_*size_x_];
}

ImgBuffer_t::~ImgBuffer_t() {
    delete [] img_;
}

void ImgBuffer_t::reset() {
    updates_ = 0;
    for (size_t j = 0; j < size_y_*size_x_; ++j) {
        img_[j] = Vec3f();
    }
}

void ImgBuffer_t::update(const Vec3f* img, size_t size_x, size_t size_y) {
    updates_++;

    for (size_t j = 0; j < size_y*size_x; ++j) {
        //img_[j][i] = img_[j][i] * (1.0 - 1.0/static_cast<double>(updates_)) + img[j][i]/static_cast<double>(updates_);
        img_[j] += img[j];
    }
}

void ImgBuffer_t::update() {
    ++updates_;
}

void ImgBuffer_t::update(const Vec3f &colour, size_t pos_x, size_t pos_y) {
    //img_[pos_y][pos_x] = img_[pos_y][pos_x] * (1.0 - 1.0/static_cast<double>(updates_)) + colour/static_cast<double>(updates_);
    img_[pos_y*size_x_ + pos_x] += colour;
}

void ImgBuffer_t::set(const Vec3f* img, size_t size_x, size_t size_y) {
    updates_ = 1;
    for (size_t j = 0; j < size_y*size_x; ++j) {
        img_[j] = img[j];
    }
}

void ImgBuffer_t::set(const Vec3f &colour, size_t pos_x, size_t pos_y) {
    img_[pos_y*size_x_ + pos_x] = colour;
}

void ImgBuffer_t::write(std::string &filename) const {
    const double update_mult = 1.0/static_cast<double>(updates_);
    cimg_library::CImg<unsigned short> image(static_cast<unsigned int>(size_x_), static_cast<unsigned int>(size_y_), 1, 3);
    const unsigned int n = static_cast<unsigned int>(size_x_ * size_y_);

    //constexpr unsigned int bit_depth = 16;
    //constexpr double bit_multiplier = std::pow(2.0, bit_depth) - 1.0;
    constexpr double bit_multiplier = 65535.0; // 16 bits, with msvc std::pow is not constexpr :(

    for (size_t j = 0; j < size_y_; ++j) {
        for (size_t i = 0; i < size_x_; ++i) {
            Vec3f colour = img_[j*size_x_ + i]*update_mult;
            colour.clamp(0.0, 1.0);
            colour *= bit_multiplier;
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 0, n, n) = static_cast<unsigned short>(std::lround(colour[0]));
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 1, n, n) = static_cast<unsigned short>(std::lround(colour[1]));
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 2, n, n) = static_cast<unsigned short>(std::lround(colour[2]));
        }
    }

    image.save_png(filename.c_str());
}

void ImgBuffer_t::write(std::string &filename, double gammaind) const {
    const double update_mult = 1.0/static_cast<double>(updates_);
    cimg_library::CImg<unsigned short> image(static_cast<unsigned int>(size_x_), static_cast<unsigned int>(size_y_), 1, 3);
    const unsigned int n = static_cast<unsigned int>(size_x_ * size_y_);

    //constexpr unsigned int bit_depth = 16;
    //constexpr double bit_multiplier = std::pow(2.0, bit_depth) - 1.0;
    constexpr double bit_multiplier = 65535.0; // 16 bits, with msvc std::pow is not constexpr :(

    for (size_t j = 0; j < size_y_; ++j) {
        for (size_t i = 0; i < size_x_; ++i) {
            Vec3f colour = img_[j*size_x_ + i]*update_mult;
            colour.clamp(0.0, 1.0).pow_inplace(gammaind);
            colour *= bit_multiplier;
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 0, n, n) = static_cast<unsigned short>(std::lround(colour[0]));
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 1, n, n) = static_cast<unsigned short>(std::lround(colour[1]));
            image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 2, n, n) = static_cast<unsigned short>(std::lround(colour[2]));
        }
    }

    image.save_png(filename.c_str());
}