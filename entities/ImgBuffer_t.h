#ifndef IMGBUFFER_T_H
#define IMGBUFFER_T_H

#include "Vec3f.h"
#include <string>

class ImgBuffer_t{
    public:
        ImgBuffer_t(unsigned int size_x, unsigned int size_y);
        ~ImgBuffer_t();

        void reset();
        void update(const Vec3f** img, unsigned int size_x, unsigned int size_y);
        void update();
        void update(const Vec3f &colour, unsigned int pos_x, unsigned int pos_y);
        void set(const Vec3f** img, unsigned int size_x, unsigned int size_y);
        void write(std::string &filename) const;

        unsigned int size_x_;
        unsigned int size_y_;
        unsigned int updates_;
        Vec3f** img_;
};
#endif