#include "Ray_t.h"
#include "Medium_t.h"
#include "Scene_t.h"
#include "Skybox_t.h"
#include "Shape_t.h"
#include "ScatteringFunction_t.h"

Ray_t::Ray_t(const Vec3f &origin, const Vec3f &direction, const Vec3f &colour, const Vec3f &mask, const std::list<Medium_t*> &medium_list, double time /*= 0.0*/) : 
    origin_(origin), direction_(direction), colour_(colour), mask_(mask), dist_(0.0), medium_list_(medium_list), time_(time) {}

Ray_t::~Ray_t(){}

void Ray_t::raycast(const Scene_t* scene, unsigned int max_bounces, const Skybox_t* skybox){
    unsigned int bounces = 0;
    Shape_t* hit_obj;
    double t;
    double uv[2];
    bool scattered;

    while ((bounces < max_bounces) && (mask_.magnitudeSquared() > 0.01)){ // Should maybe make magnitudesquared min value lower
        hit_obj = nullptr;
        scattered = false;
        scene->intersect(*this, hit_obj, t, uv);
        dist_ = t;
        if (hit_obj == nullptr){
            colour_ += mask_ * skybox->get(direction_);
            return;
        }
        bounces++;

        medium_list_.front()->scattering_->scatter(*this, scattered);
        if (!scattered){
            hit_obj->material_->bounce(uv, hit_obj, *this);
        }
    }
}

void Ray_t::add_to_mediums(Medium_t* medium){
    for (std::list<Medium_t*>::iterator i = medium_list_.begin(); i != medium_list_.end(); i++){
        if ((*i)->priority_ <= medium->priority_){
            medium_list_.insert(i, medium);
            return;
        }
    }
    medium_list_.push_back(medium);
}

void Ray_t::remove_from_mediums(Medium_t* medium){
    for (std::list<Medium_t*>::iterator i = medium_list_.begin(); i != medium_list_.end(); i++){
        if (*i == medium){
            medium_list_.erase(i);
            return;
        }
    }
}