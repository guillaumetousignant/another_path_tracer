#include "PortalScatterer_t.h"
#include "TransformMatrix_t.h"
#include "Medium_t.h"
#include "RandomGenerator_t.h"

using APTracer::Materials::PortalScatterer_t;

PortalScatterer_t::PortalScatterer_t(TransformMatrix_t* transformation, double scattering_distance, std::list<Medium_t*> medium_list)
    : PortalScattererTop_t(transformation, medium_list), scattering_coefficient_(1/scattering_distance), unif_(0.0, 1.0) {}

PortalScatterer_t::~PortalScatterer_t() {}

bool PortalScatterer_t::scatter(Ray_t &ray) {
    const double distance = -std::log(unif_(APTracer::Entities::rng))/scattering_coefficient_;

    if (distance >= ray.dist_){
        return false;
    }
    else{
        ray.dist_ = distance;
        ray.origin_ = transformation_->multVec(ray.origin_ + ray.direction_ * ray.dist_);
        ray.direction_ = transformation_->transformDir().multDir(ray.direction_).normalize_inplace(); // CHECK cache transformation_->transformDir()??
        ray.medium_list_ = medium_list_;
        return true;
    }
}