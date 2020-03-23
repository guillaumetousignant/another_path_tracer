#ifndef BOX_T_H
#define BOX_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;

namespace APTracer::Shapes {
    class Box_t {
        public:
            Box_t();
            Box_t(Vec3f (&coord)[2]);
            ~Box_t();

            Vec3f coordinates_[2];

            bool intersection(const Ray_t &ray, double &t) const;
    };
}
#endif