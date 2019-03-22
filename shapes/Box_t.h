#ifndef BOX_T_H
#define BOX_T_H

#include "Ray_t.h"
#include "Vec3f.h"

class Box_t {
    public:
        Box_t(Vec3f (&coord)[2]);
        ~Box_t();

        Vec3f coordinates_[2];

        void intersection(const Ray_t &ray, bool &intersected, double &t) const;
};
#endif