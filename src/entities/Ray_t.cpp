#include "entities/Ray_t.h"
#include "entities/Material_t.h"
#include "entities/Medium_t.h"
#include "entities/Scene_t.h"
#include "entities/Skybox_t.h"
#include "entities/Shape_t.h"

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Shape_t;

Ray_t::Ray_t(const Vec3f &origin, const Vec3f &direction, const Vec3f &colour, const Vec3f &mask, std::list<Medium_t*> medium_list, double time /*= 1.0*/) : 
    origin_(origin), direction_(direction), colour_(colour), mask_(mask), dist_(0.0), medium_list_(std::move(medium_list)), time_(time) {}

Ray_t::~Ray_t() = default;

void Ray_t::raycast(const Scene_t* scene, unsigned int max_bounces, const Skybox_t* skybox) {
    unsigned int bounces = 0;

    while ((bounces < max_bounces) && (mask_.magnitudeSquared() > 0.01)) { // Should maybe make magnitudesquared min value lower
        double t;
        std::array<double, 2> uv;

        const Shape_t* hit_obj = scene->intersect(*this, t, uv);
        
        if (hit_obj == nullptr) {
            colour_ += mask_ * skybox->get(direction_);
            return;
        }
        dist_ = t;
        bounces++;

        if (!medium_list_.front()->scatter(*this)) {
            hit_obj->material_->bounce(uv, hit_obj, *this);
        }
    }
}

void Ray_t::add_to_mediums(Medium_t* medium) {
    for (auto i = medium_list_.begin(); i != medium_list_.end(); ++i) {
        if ((*i)->priority_ <= medium->priority_) {
            medium_list_.insert(i, medium);
            return;
        }
    }
    medium_list_.push_back(medium);
}

void Ray_t::remove_from_mediums(Medium_t* medium) {
    for (auto i = medium_list_.begin(); i != medium_list_.end(); ++i) {
        if (*i == medium) {
            medium_list_.erase(i);
            return;
        }
    }
}