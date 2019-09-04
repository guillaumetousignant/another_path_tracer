#include "PortalScatterer_t.h"
#include "TransformMatrix_t.h"
#include "Medium_t.h"
#include "RandomGenerator_t.h"

PortalScatterer_t::PortalScatterer_t(TransformMatrix_t* transformation, double scattering_distance, std::list<Medium_t*> medium_list)
    : PortalScattererTop_t(transformation, medium_list), scattering_coefficient_(1/scattering_distance), unif_(0.0, 1.0) {}

PortalScatterer_t::~PortalScatterer_t() {}

void PortalScatterer_t::scatter(Ray_t &ray, bool &intersected) {
    const double distance = -std::log(unif_(my_rand::rng))/scattering_coefficient_;

    if (distance >= ray.dist_){
        intersected = false;
    }
    else{
        intersected = true;
        ray.dist_ = distance;

        const TransformMatrix_t transform_norm = transformation_->transformDir();

        ray.origin_ = transformation_->multVec(ray.origin_ + ray.direction_ * ray.dist_);
        ray.direction_ = transform_norm.multDir(ray.direction_).normalize();

        ray.medium_list_ = medium_list_;
    }
}