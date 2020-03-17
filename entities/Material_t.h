#ifndef MATERIAL_T_H
#define MATERIAL_T_H

#include "Ray_t.h"

namespace APTracer::Entities {
    class Shape_t;

    class Material_t{
        public:
            Material_t(){};
            virtual ~Material_t(){};

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) = 0;
    };
}

#endif