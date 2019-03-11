#include "Portal_t.h"
#include "Shape_t.h"
#include "Vec3f.h"
#include "Medium_t.h"
#include "TransformMatrix_t.h"

#define EPSILON 0.00000001

Portal_t::Portal_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list) 
    : transformation_(transformation), medium_list_(medium_list) {}

Portal_t::~Portal_t(){}

void Portal_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    hit_obj->normal(ray, uv, normal);

    if (ray.direction_.dot(normal) < 0){
        Vec3f newdir;
        TransformMatrix_t transform_norm = transformation_->transformDir();

        ray.origin_ = transformation_->multVec(ray.origin_ + ray.direction_ * ray.dist_);
        ray.direction_ = transform_norm.multDir(ray.direction_).normalize();
        ray.medium_list_ = medium_list_;
    }
    else{
        ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    }
}