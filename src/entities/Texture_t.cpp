#include "entities/Texture_t.hpp"
#include <cmath>
#include <string>
#ifdef APTRACER_USE_JPEG
    #define cimg_use_jpeg
#endif
#ifdef APTRACER_USE_PNG
    #define cimg_use_png
#endif
#ifdef APTRACER_USE_TIFF
    #define cimg_use_tiff
#endif
#define cimg_use_tinyexr
#ifdef APTRACER_USE_OPENMP
    #define TINYEXR_USE_OPENMP (1)
    #define cimg_use_openmp 1
#endif
#define cimg_use_cpp11 1
#define cimg_display 0
#include "functions/CImg.h"

using APTracer::Entities::Texture_t;
using APTracer::Entities::Vec3f;

Texture_t::Texture_t(const std::string& filename) {
    cimg_library::CImg<double> image;
    std::string extension = filename.substr(filename.find_last_of('.') + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension == "jpeg" || extension == "jpg") {
        image.load_jpeg(filename.c_str());
        // 1/(std::pow(2.0, bit_depth) - 1.0)
        constexpr double bit_multiplier = 1 / 255.0; // 8 bits, with msvc std::pow is not constexpr :(
        image *= bit_multiplier; // Normalizing by bit depth
    }
    else if (extension == "png") {
        unsigned int bit_depth = 8;
        image.load_png(filename.c_str(), &bit_depth);
        image /= (std::pow(2.0, bit_depth) - 1.0); // Normalizing by bit depth
    }
    else if (extension == "exr") {
        image.load_exr(filename.c_str());
        image.pow(1.0 / 2.2); // Gamma correcting
    }
    else if (extension == "hdr") {
        image.load(filename.c_str());
        // 1/(std::pow(2.0, bit_depth) - 1.0)
        constexpr double bit_multiplier = 1 / 65535.0; // 16 bits, with msvc std::pow is not constexpr :(
        image *= bit_multiplier; // Normalizing by bit depth
    }
    else {
        image.load(filename.c_str());
        // 1/(std::pow(2.0, bit_depth) - 1.0)
        constexpr double bit_multiplier = 1 / 255.0; // 8 bits, with msvc std::pow is not constexpr :(
        image *= bit_multiplier; // Normalizing by bit depth
    }

    size_x_ = image.width();
    size_y_ = image.height();

    img_ = std::vector<Vec3f>(size_y_ * size_x_);

    const auto n = static_cast<unsigned int>(size_x_ * size_y_);

    for (size_t j = 0; j < size_y_; ++j) {
        for (size_t i = 0; i < size_x_; ++i) {
            img_[(size_y_ - j - 1) * size_x_ + i] = Vec3f(image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 0, n, n),
                                                          image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 1, n, n),
                                                          image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 2, n, n));
        }
    }
}

auto Texture_t::get(std::array<double, 2> xy) const -> Vec3f {
    const double x = (size_x_ - 1) * (xy[0] - std::floor(xy[0]));
    const double y = (size_y_ - 1) * (xy[1] - std::floor(xy[1]));

    const double xd = x - std::floor(x);
    const double yd = y - std::floor(y);

    const size_t x0 = static_cast<size_t>(std::max(x - 1.0, 0.0)); // floor
    const size_t x1 = static_cast<size_t>(x); // floor
    const size_t x2 = static_cast<size_t>(x + 1.0); // ceil
    const size_t x3 = std::min(static_cast<size_t>(x + 2.0), size_x_ - 1); // ceil
    const size_t y0 = static_cast<size_t>(std::max(y - 1.0, 0.0)); // floor
    const size_t y1 = static_cast<size_t>(y); // floor
    const size_t y2 = static_cast<size_t>(y + 1.0); // ceil
    const size_t y3 = std::min(static_cast<size_t>(y + 2.0), size_y_ - 1); // ceil

    const Vec3f cp0x = CubicHermite(img_[y0 * size_x_ + x0], img_[y0 * size_x_ + x1], img_[y0 * size_x_ + x2], img_[y0 * size_x_ + x3], xd);
    const Vec3f cp1x = CubicHermite(img_[y1 * size_x_ + x0], img_[y1 * size_x_ + x1], img_[y1 * size_x_ + x2], img_[y1 * size_x_ + x3], xd);
    const Vec3f cp2x = CubicHermite(img_[y2 * size_x_ + x0], img_[y2 * size_x_ + x1], img_[y2 * size_x_ + x2], img_[y2 * size_x_ + x3], xd);
    const Vec3f cp3x = CubicHermite(img_[y3 * size_x_ + x0], img_[y3 * size_x_ + x1], img_[y3 * size_x_ + x2], img_[y3 * size_x_ + x3], xd);
    return CubicHermite(cp0x, cp1x, cp2x, cp3x, yd);
}

// From demofox on Shadertoy, https://www.shadertoy.com/view/MllSzX
auto Texture_t::get_cubic(std::array<double, 2> xy) const -> Vec3f {
    const double x = (size_x_ - 1) * (xy[0] - std::floor(xy[0]));
    const double y = (size_y_ - 1) * (xy[1] - std::floor(xy[1]));

    const double xd = x - std::floor(x);
    const double yd = y - std::floor(y);

    const size_t x0 = static_cast<size_t>(std::max(x - 1.0, 0.0)); // floor
    const size_t x1 = static_cast<size_t>(x); // floor
    const size_t x2 = static_cast<size_t>(x + 1.0); // ceil
    const size_t x3 = std::min(static_cast<size_t>(x + 2.0), size_x_ - 1); // ceil
    const size_t y0 = static_cast<size_t>(std::max(y - 1.0, 0.0)); // floor
    const size_t y1 = static_cast<size_t>(y); // floor
    const size_t y2 = static_cast<size_t>(y + 1.0); // ceil
    const size_t y3 = std::min(static_cast<size_t>(y + 2.0), size_y_ - 1); // ceil

    const Vec3f cp0x = CubicHermite(img_[y0 * size_x_ + x0], img_[y0 * size_x_ + x1], img_[y0 * size_x_ + x2], img_[y0 * size_x_ + x3], xd);
    const Vec3f cp1x = CubicHermite(img_[y1 * size_x_ + x0], img_[y1 * size_x_ + x1], img_[y1 * size_x_ + x2], img_[y1 * size_x_ + x3], xd);
    const Vec3f cp2x = CubicHermite(img_[y2 * size_x_ + x0], img_[y2 * size_x_ + x1], img_[y2 * size_x_ + x2], img_[y2 * size_x_ + x3], xd);
    const Vec3f cp3x = CubicHermite(img_[y3 * size_x_ + x0], img_[y3 * size_x_ + x1], img_[y3 * size_x_ + x2], img_[y3 * size_x_ + x3], xd);
    return CubicHermite(cp0x, cp1x, cp2x, cp3x, yd);
}

auto Texture_t::get_linear(std::array<double, 2> xy) const -> Vec3f {
    const double x = (size_x_ - 1) * (xy[0] - std::floor(xy[0]));
    const double y = (size_y_ - 1) * (xy[1] - std::floor(xy[1]));

    const double xd = x - std::floor(x);
    const double yd = y - std::floor(y);

    const size_t xlo = static_cast<size_t>(x); // floor
    const size_t xhi = static_cast<size_t>(x + 1.0); // ceil
    const size_t ylo = static_cast<size_t>(y); // floor
    const size_t yhi = static_cast<size_t>(y + 1.0); // ceil

    return img_[ylo * size_x_ + xlo] * (1.0 - xd) * (1.0 - yd) + img_[ylo * size_x_ + xhi] * xd * (1.0 - yd) + img_[yhi * size_x_ + xlo] * (1.0 - xd) * yd + img_[yhi * size_x_ + xhi] * xd * yd;
}

auto Texture_t::get_nn(std::array<double, 2> xy) const -> Vec3f {
    return img_[std::lround((size_y_ - 1) * (xy[1] - std::floor(xy[1]))) * size_x_ + std::lround((size_x_ - 1) * (xy[0] - std::floor(xy[0])))];
}

// From demofox on Shadertoy, https://www.shadertoy.com/view/MllSzX
auto Texture_t::CubicHermite(Vec3f first, Vec3f second, Vec3f third, Vec3f fourth, double t) -> Vec3f {
    const double t2 = t * t;
    const double t3 = t * t * t;
    const Vec3f a   = -first / 2.0 + 3.0 * second / 2.0 - 3.0 * third / 2.0 + fourth / 2.0;
    const Vec3f b   = first - 5.0 * second / 2.0 + 2.0 * third - fourth / 2.0;
    const Vec3f c   = -first / 2.0 + third / 2.0;

    return a * t3 + b * t2 + c * t + second;
}