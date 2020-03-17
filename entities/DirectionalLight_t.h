#ifndef DIRECTIONALLIGHT_T_H
#define DIRECTIONALLIGHT_T_H

#include "Vec3f.h"

namespace APTracer::Entities {
    class TransformMatrix_t;

    class DirectionalLight_t{
        public:
            DirectionalLight_t(const Vec3f &intensity, TransformMatrix_t* transformation);
            ~DirectionalLight_t();

            Vec3f direction_;
            Vec3f intensity_;
            TransformMatrix_t* transformation_;
            double radius_;

            void update();
    };
}

#endif