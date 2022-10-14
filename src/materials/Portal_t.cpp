#include "materials/Portal_t.h"
#include "entities/Medium_t.h"
#include "entities/Shape_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Vec3f.h"

constexpr double epsilon = 0.00000001;

using APTracer::Entities::Vec3f;

APTracer::Materials::Portal_t::Portal_t(APTracer::Entities::TransformMatrix_t* transformation, std::list<APTracer::Entities::Medium_t*> medium_list) :
        transformation_(transformation), medium_list_(std::move(medium_list)) {}

auto APTracer::Materials::Portal_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    const Vec3f normal = hit_obj->normal(ray.time_, uv);

    if (ray.direction_.dot(normal) < 0) {
        ray.origin_      = transformation_->multVec(ray.origin_ + ray.direction_ * ray.dist_);
        ray.direction_   = transformation_->multDir(ray.direction_);
        ray.medium_list_ = medium_list_;
    }
    else {
        ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon;
    }
}