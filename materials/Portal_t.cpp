#include "Portal_t.h"
#include "Shape_t.h"
#include "Vec3f.h"
#include "Medium_t.h"
#include "TransformMatrix_t.h"

#define EPSILON 0.00000001

using APTracer::Materials::Portal_t;

Portal_t::Portal_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list) 
    : PortalTop_t(transformation, medium_list) {}

Portal_t::~Portal_t(){}

void Portal_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    hit_obj->normal(ray, uv, normal);

    if (ray.direction_.dot(normal) < 0){
        ray.origin_ = transformation_->multVec(ray.origin_ + ray.direction_ * ray.dist_);
        ray.direction_ = transformation_->transformDir().multDir(ray.direction_).normalize_inplace(); // CHECK cache transformation_->transformDir()??
        ray.medium_list_ = medium_list_;
    }
    else{
        ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    }
}