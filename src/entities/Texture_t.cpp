#include "entities/Texture_t.h"
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

Texture_t::Texture_t(const std::string &filename) {
    cimg_library::CImg<double> image;
    std::string extension = filename.substr(filename.find_last_of('.') + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension == "jpeg" || extension == "jpg") { 
        image.load_jpeg(filename.c_str());
        constexpr unsigned int bit_depth = 8;
        image/=(std::pow(2.0, bit_depth) - 1.0); // Normalizing by bit depth
    } else if (extension == "png") {
        unsigned int bit_depth;
        image.load_png(filename.c_str(), &bit_depth);
        image/=(std::pow(2.0, bit_depth) - 1.0); // Normalizing by bit depth
    } else if (extension == "exr") {
        image.load_exr(filename.c_str());
        image.pow(1.0/2.2); // Gamma correcting
    } else if (extension == "hdr") {
        image.load(filename.c_str());
        constexpr unsigned int bit_depth = 16; 
        image/=(std::pow(2.0, bit_depth) - 1.0); // Normalizing by bit depth
    } else {
        image.load(filename.c_str());
        constexpr unsigned int bit_depth = 8; 
        image/=(std::pow(2.0, bit_depth) - 1.0); // Normalizing by bit depth
    }

    size_x_ = image.width();
    size_y_ = image.height();

    img_ = std::vector<Vec3f>(size_y_*size_x_);

    const auto n = static_cast<unsigned int>(size_x_ * size_y_);

    for (size_t j = 0; j < size_y_; ++j) {
        for (size_t i = 0; i < size_x_; ++i) {
            //img_[j][i] = Vec3f(image(i, j, 0), image(i, j, 1), image(i, j, 2));
            img_[(size_y_ - j - 1)*size_x_ + i] = Vec3f(image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 0, n, n), image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 1, n, n), image(static_cast<unsigned int>(i), static_cast<unsigned int>(j), 0, 2, n, n));
        }
    }
}

auto Texture_t::get(std::array<double, 2> xy) const -> Vec3f {
    const double x = (size_x_ - 1) * xy[0];
    const double y = (size_y_ - 1) * xy[1];

    const double xd = x - std::floor(x);
    const double yd = y - std::floor(y);

    const int xlo = static_cast<int>(x);       // floor
    const int xhi = static_cast<int>(x + 1.0); // ceil
    const int ylo = static_cast<int>(y);       // floor
    const int yhi = static_cast<int>(y + 1.0); // ceil

    return  img_[ylo*size_x_ + xlo] * (1.0 - xd) * (1.0 - yd) +
            img_[ylo*size_x_ + xhi] * xd * (1.0 - yd) + 
            img_[yhi*size_x_ + xlo] * (1.0 - xd) * yd +
            img_[ yhi*size_x_ + xhi] * xd * yd;
}

auto Texture_t::get_nn(std::array<double, 2> xy) const -> Vec3f {
    return img_[std::lround((size_x_ - 1) * xy[0])* size_x_ + std::lround((size_y_ - 1) * xy[1])];
}