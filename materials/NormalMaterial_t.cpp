#include "NormalMaterial_t.h"
#include "Shape_t.h"
#include "Vec3f.h"

NormalMaterial_t::NormalMaterial_t() {}

NormalMaterial_t::~NormalMaterial_t(){}

void NormalMaterial_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;

    hit_obj->normal(ray, uv, normal);
    ray.colour_ = normal*0.5 + 0.5;
    ray.mask_ = Vec3f();
}