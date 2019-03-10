#ifndef REFLECTIVE_T_H
#define REFLECTIVE_T_H

#include "Material_t.h"
#include "Vec3f.h"

class Reflective_t : public Material_t{
    public:
        Reflective_t(const Vec3f &emission, const Vec3f &colour);
        ~Reflective_t();

        Vec3f emission_;
        Vec3f colour_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif