#include "Texture_t.h"
#include <string>
#include "CImg.h"

Texture_t::Texture_t(const std::string &filename){
    cimg_library::CImg<unsigned char> image(filename.c_str());

    size_x_ = image.width();
    size_y_ = image.height();

    img_ = new Vec3f*[size_y_];

    for (unsigned int j = 0; j < size_y_; j++){
        img_[j] = new Vec3f[size_x_];
    }

    unsigned int n = size_x_ * size_y_;

    for (unsigned int j = 0; j < size_y_; j++){
        for (unsigned int i = 0; i < size_x_; i++){
            //img_[j][i] = Vec3f(image(i, j, 0), image(i, j, 1), image(i, j, 2));
            img_[j][i] = Vec3f(image(i, j, 0, 0, n, n)/255.0, image(i, j, 0, 1, n, n)/255.0, image(i, j, 0, 2, n, n)/255.0);
        }
    }
}

Texture_t::~Texture_t(){
    if (img_ != nullptr){
        for (unsigned int j = 0; j < size_y_; j++){
            if (img_[j] != nullptr){
                delete [] img_[j];
            }
        }
        delete [] img_;
    }
}

Vec3f Texture_t::get(double (&xy)[2]) const {
    double x, y;
    double xd, yd;
    unsigned int xlo, xhi, ylo, yhi;

    x = ((double)size_x_ - 1.0) * xy[0];
    y = ((double)size_y_ - 1.0) * xy[1];

    xd = x - std::floor(x);
    yd = y - std::floor(y);

    xlo = (unsigned int)std::floor(x);
    xhi = (unsigned int)std::ceil(x);
    ylo = (unsigned int)std::floor(y);
    yhi = (unsigned int)std::ceil(y);


    return  img_[ylo][xlo] * (1.0 - xd) * (1.0 - yd) +
            img_[ylo][xhi] * xd * (1.0 - yd) + 
            img_[yhi][xlo] * (1.0 - xd) * yd +
            img_[ yhi][xhi] * xd * yd;
}

Vec3f Texture_t::get_nn(double (&xy)[2]) const {
    return img_[(unsigned int)std::round(((double)size_x_ - 1.0) * xy[0])][(unsigned int)std::round(((double)size_y_ - 1.0) * xy[1])];
}