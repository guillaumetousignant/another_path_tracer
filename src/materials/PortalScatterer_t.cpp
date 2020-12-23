#include "materials/PortalScatterer_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/RandomGenerator_t.h"

APTracer::Materials::PortalScatterer_t::PortalScatterer_t(APTracer::Entities::TransformMatrix_t* transformation, double scattering_distance, std::list<APTracer::Entities::Medium_t*> medium_list, double ind, unsigned int priority)
    : PortalScattererTop_t(transformation, medium_list, ind, priority), scattering_coefficient_(1/scattering_distance), unif_(0.0, 1.0) {}

APTracer::Materials::PortalScatterer_t::~PortalScatterer_t() {}

bool APTracer::Materials::PortalScatterer_t::scatter(APTracer::Entities::Ray_t &ray) {
    const double distance = -std::log(unif_(APTracer::Entities::rng))/scattering_coefficient_;

    if (distance >= ray.dist_) {
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