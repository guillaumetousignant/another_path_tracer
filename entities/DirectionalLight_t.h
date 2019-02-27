#ifndef DIRECTIONALLIGHT_T_H
#define DIRECTIONALLIGHT_T_H

#include "Vec3f.h"
#include "TransformMatrix_t.h"

class DirectionalLight_t{
    public:
        DirectionalLight_t(Vec3f intensity, TransformMatrix_t transformation){};
        ~DirectionalLight_t(){};

        Vec3f direction_;
        Vec3f intensity_;
        TransformMatrix_t transformation_;
        double radius_;

        void update(){};
};

#endif