#ifndef TEXTURE_T_H
#define TEXTURE_T_H

#include "Vec3f.h"
#include <string>

class Texture_t{
    public:
        Texture_t(const std::string &filename);
        ~Texture_t();

        unsigned int size_x_;
        unsigned int size_y_;
        Vec3f** img_;

        Vec3f get(double (&xy)[2]) const;
        Vec3f get_nn(double (&xy)[2]) const;
};

#endif