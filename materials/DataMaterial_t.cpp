#include "DataMaterial_t.h"
#include "Shape_t.h"

DataMaterial_t::DataMaterial_t(Vec3f* data) : data_(data) {}

DataMaterial_t::~DataMaterial_t(){}

void DataMaterial_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    ray.colour_ = data_[hit_obj->id_];
    ray.mask_ = Vec3f();
}