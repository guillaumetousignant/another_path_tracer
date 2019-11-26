#ifndef DATAMATERIAL_T_H
#define DATAMATERIAL_T_H

#include "Material_t.h"
#include "Vec3f.h"

class DataMaterial_t final : public Material_t{
    public:
        DataMaterial_t(Vec3f* data);
        virtual ~DataMaterial_t() final;

        Vec3f* data_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
};
#endif