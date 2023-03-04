#include "materials/PortalScatterer_t.hpp"
#include "entities/RandomGenerator_t.hpp"

APTracer::Materials::PortalScatterer_t::PortalScatterer_t(
    APTracer::Entities::TransformMatrix_t* transformation, std::list<APTracer::Entities::Medium_t*> medium_list, double scattering_distance, double ind, unsigned int priority) :
        Medium_t(ind, priority), transformation_(transformation), medium_list_(std::move(medium_list)), scattering_coefficient_(1 / scattering_distance), unif_(0.0, 1.0) {}

auto APTracer::Materials::PortalScatterer_t::scatter(APTracer::Entities::Ray_t& ray) -> bool {
    const double distance = -std::log(unif_(APTracer::Entities::rng())) / scattering_coefficient_;

    if (distance >= ray.dist_) {
        return false;
    }

    ray.dist_        = distance;
    ray.origin_      = transformation_->multVec(ray.origin_ + ray.direction_ * ray.dist_);
    ray.direction_   = transformation_->multDir(ray.direction_);
    ray.medium_list_ = medium_list_;
    return true;
}
