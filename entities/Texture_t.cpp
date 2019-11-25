#include "Texture_t.h"
#include <string>
#define cimg_use_jpeg
#define cimg_use_png
#define cimg_use_tiff
#define cimg_display 0
#include "CImg.h"

Texture_t::Texture_t(const std::string &filename){
    const cimg_library::CImg<unsigned char> image(filename.c_str());

    size_x_ = image.width();
    size_y_ = image.height();

    img_ = new Vec3f[size_y_*size_x_];

    const unsigned int n = size_x_ * size_y_;

    for (unsigned int j = 0; j < size_y_; ++j){
        for (unsigned int i = 0; i < size_x_; ++i){
            //img_[j][i] = Vec3f(image(i, j, 0), image(i, j, 1), image(i, j, 2));
            img_[(size_y_ - j - 1)*size_x_ + i] = Vec3f(image(i, j, 0, 0, n, n)/255.0, image(i, j, 0, 1, n, n)/255.0, image(i, j, 0, 2, n, n)/255.0);
        }
    }
}

Texture_t::~Texture_t(){
    if (img_ != nullptr){
        delete [] img_;
    }
}

Vec3f Texture_t::get(const double (&xy)[2]) const {
    const double x = (size_x_ - 1) * xy[0];
    const double y = (size_y_ - 1) * xy[1];

    const double xd = x - std::floor(x);
    const double yd = y - std::floor(y);

    const int xlo = x;      // floor
    const int xhi = x + 1.0;// ceil
    const int ylo = y;      // floor
    const int yhi = y + 1.0;// ceil

    return  img_[ylo*size_x_ + xlo] * (1.0 - xd) * (1.0 - yd) +
            img_[ylo*size_x_ + xhi] * xd * (1.0 - yd) + 
            img_[yhi*size_x_ + xlo] * (1.0 - xd) * yd +
            img_[ yhi*size_x_ + xhi] * xd * yd;
}

Vec3f Texture_t::get_nn(const double (&xy)[2]) const {
    return img_[std::lround((size_x_ - 1) * xy[0])* size_x_ + std::lround((size_y_ - 1) * xy[1])];
}