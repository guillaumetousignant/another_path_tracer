#include "entities/Ray_t.hpp"
#include "entities/Medium_t.hpp"

using APTracer::Entities::Ray_t;

Ray_t::Ray_t(const Vec3f& origin, const Vec3f& direction, const Vec3f& colour, const Vec3f& mask, std::list<Medium_t*> medium_list) :
        origin_(origin), direction_(direction), colour_(colour), mask_(mask), dist_(0.0), medium_list_(std::move(medium_list)), time_(1.0) {}

Ray_t::Ray_t(const Vec3f& origin, const Vec3f& direction, const Vec3f& colour, const Vec3f& mask, std::list<Medium_t*> medium_list, double time) :
        origin_(origin), direction_(direction), colour_(colour), mask_(mask), dist_(0.0), medium_list_(std::move(medium_list)), time_(time) {}

auto Ray_t::add_to_mediums(Medium_t* medium) -> void {
    for (auto i = medium_list_.begin(); i != medium_list_.end(); ++i) {
        if ((*i)->priority_ <= medium->priority_) {
            medium_list_.insert(i, medium);
            return;
        }
    }
    medium_list_.push_back(medium);
}

auto Ray_t::remove_from_mediums(Medium_t* medium) -> void {
    for (auto i = medium_list_.begin(); i != medium_list_.end(); ++i) {
        if (*i == medium) {
            medium_list_.erase(i);
            return;
        }
    }
}