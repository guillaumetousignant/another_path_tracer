#ifndef SKYBOX_T_H
#define SKYBOX_T_H

#include "Vec3f.h"

class Skybox_t{
    public:
        Skybox_t(){};
        ~Skybox_t(){};

        virtual Vec3f get(Vec3f xyz) const = 0;
};

#endif