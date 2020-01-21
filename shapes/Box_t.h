#ifndef BOX_T_H
#define BOX_T_H

#include "Ray_t.h"
#include "Vec3f.h"

class Box_t {
    public:
        Box_t();
        Box_t(Vec3f (&coord)[2]);
        ~Box_t();

        Vec3f coordinates_[2];

        bool intersection(const Ray_t &ray, double &t) const;
};
#endif