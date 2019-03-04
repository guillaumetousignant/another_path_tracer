#ifndef TEXTURE_T_H
#define TEXTURE_T_H

#include "Vec3f.h"

class Texture_t{
    public:
        Texture_t(){};
        ~Texture_t(){};

        unsigned int size_x_;
        unsigned int size_y_;
        Vec3f** img_;

        Vec3f get(unsigned int (&xy)[2]) const {};
        Vec3f get_nn(unsigned int (&xy)[2]) const {};
};

#endif