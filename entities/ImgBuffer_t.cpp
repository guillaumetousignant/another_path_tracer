#include "ImgBuffer_t.h"

ImgBuffer_t::ImgBuffer_t(unsigned int size_x, unsigned int size_y): size_x_(size_x), size_y_(size_y), updates_(0) {
    img_ = new Vec3f*[size_x_];
    for (unsigned int i = 0; i < size_x_; i++){
        img_[i] = new Vec3f[size_y_];
    }
};

ImgBuffer_t::~ImgBuffer_t(){
    if (img_ != nullptr){
        for (unsigned int i = 0; i < size_x_; i++){
            if (img_[i] != nullptr){
                delete [] img_[i];
            }
        }
        delete [] img_;
    }
};

void ImgBuffer_t::reset(){
    updates_ = 0;
};

void ImgBuffer_t::update(const Vec3f** img, unsigned int size_x, unsigned int size_y){
    updates_++;

    for (unsigned int i = 0; i < size_x; i++){
        for (unsigned int j = 0; j < size_y; j++){
            img_[i][j] = img_[i][j] * (1 - 1/updates_) + img[i][j]/updates_;
        }
    }
};

void ImgBuffer_t::set(const Vec3f** img, unsigned int size_x, unsigned int size_y){
    updates_ = 1;
    for (unsigned int i = 0; i < size_x; i++){
        for (unsigned int j = 0; j < size_y; j++){
            img_[i][j] = img[i][j];
        }
    }
};