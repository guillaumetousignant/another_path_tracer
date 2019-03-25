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
    for (unsigned int j = 0; j < size_y_; j++){
        for (unsigned int i = 0; i < size_x_; i++){
            img_[j][i] = Vec3f();
        }
    }
}

void ImgBuffer_t::update(const Vec3f** img, unsigned int size_x, unsigned int size_y){
    updates_++;

    for (unsigned int j = 0; j < size_y; j++){
        for (unsigned int i = 0; i < size_x; i++){
            //img_[j][i] = img_[j][i] * (1.0 - 1.0/(double)updates_) + img[j][i]/(double)updates_;
            img_[j][i] += img[j][i];
        }
    }
}

void ImgBuffer_t::update() {
    updates_++;
}

void ImgBuffer_t::update(const Vec3f &colour, unsigned int pos_x, unsigned int pos_y) {
    //img_[pos_y][pos_x] = img_[pos_y][pos_x] * (1.0 - 1.0/(double)updates_) + colour/(double)updates_;
    img_[pos_y][pos_x] += colour;
}

void ImgBuffer_t::set(const Vec3f** img, unsigned int size_x, unsigned int size_y){
    updates_ = 1;
    for (unsigned int j = 0; j < size_y; j++){
        for (unsigned int i = 0; i < size_x; i++){
            img_[j][i] = img[j][i];
        }
    }
}

void ImgBuffer_t::write(std::string &filename) const {
    Vec3f colour;

    double update_mult = 1/(double)updates_;
    pngwriter png((int) size_x_, (int) size_y_, 0.0, filename.c_str());

    for (unsigned int j = 0; j < size_y_; j++){
        for (unsigned int i = 0; i < size_x_; i++){
            colour = img_[j][i]*update_mult;
            png.plot((int)i + 1, (int)size_y_ - (int)j, colour[0], colour[1], colour[2]);
        }
    }

    png.close();
}