#include "ImgBuffer_t.h"
#include "pngwriter.h"

ImgBuffer_t::ImgBuffer_t(unsigned int size_x, unsigned int size_y): size_x_(size_x), size_y_(size_y), updates_(0) {
    img_ = new Vec3f*[size_y_];
    for (unsigned int j = 0; j < size_y_; j++){
        img_[j] = new Vec3f[size_x_];
    }
}

ImgBuffer_t::~ImgBuffer_t(){
    if (img_ != nullptr){
        for (unsigned int j = 0; j < size_y_; j++){
            if (img_[j] != nullptr){
                delete [] img_[j];
            }
        }
        delete [] img_;
    }
}

void ImgBuffer_t::reset(){
    updates_ = 0;
}

void ImgBuffer_t::update(const Vec3f** img, unsigned int size_x, unsigned int size_y){
    updates_++;

    for (unsigned int j = 0; j < size_y; j++){
        for (unsigned int i = 0; i < size_x; i++){
            img_[j][i] = img_[j][i] * (1 - 1/updates_) + img[j][i]/updates_;
        }
    }
}

void ImgBuffer_t::update() {
    updates_++;
}

void ImgBuffer_t::update(const Vec3f &colour, unsigned int pos_x, unsigned int pos_y) {
    img_[pos_y][pos_x] = img_[pos_y][pos_x] * (1 - 1/updates_) + colour/updates_;
}

void ImgBuffer_t::set(const Vec3f** img, unsigned int size_x, unsigned int size_y){
    updates_ = 1;
    for (unsigned int j = 0; j < size_y; j++){
        for (unsigned int i = 0; i < size_x; i++){
            img_[j][i] = img[j][i];
        }
    }
}

void ImgBuffer_t::write(std::string filename) const {
    // What to do here?
}