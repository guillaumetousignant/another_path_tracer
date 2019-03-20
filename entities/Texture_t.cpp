#include "Texture_t.h"
#include <string>
#include "pngwriter.h"

Texture_t::Texture_t(std::string filename){
    pngwriter png(1, 1, 0.0, "");
    png.readfromfile(filename.c_str()); 

    size_x_ = png.getwidth();
    size_y_ = png.getheight();

    img_ = new Vec3f*[size_y_];

    for (unsigned int j = 0; j < size_y_; j++){
        img_[j] = new Vec3f[size_x_];
    }

    for (unsigned int j = 0; j < size_y_; j++){
        for (unsigned int i = 0; i < size_x_; i++){
            img_[j][i] = Vec3f(png.dread((int)i+1, (int)j+1, 1), png.dread((int)i+1, (int)j+1, 2), png.dread((int)i+1, (int)j+1, 3));
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


    return  img_[size_y_ - 1 - ylo][xlo] * (1.0 - xd) * (1.0 - yd) +
            img_[size_y_ - 1 - ylo][xhi] * xd * (1.0 - yd) + 
            img_[size_y_ - 1 - yhi][xlo] * (1.0 - xd) * yd +
            img_[size_y_ - 1 - yhi][xhi] * xd * yd;
}

Vec3f Texture_t::get_nn(double (&xy)[2]) const {
    return img_[size_y_ - 1 - (unsigned int)std::round(((double)size_x_ - 1.0) * xy[0])][(unsigned int)std::round(((double)size_y_ - 1.0) * xy[1])];
}