#include "Box_t.h"
#include <limits>
#include <algorithm>

using APTracer::Shapes::Box_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;

Box_t::Box_t() {}

Box_t::Box_t(Vec3f (&coord)[2]) : coordinates_{coord[0], coord[1]} {}

Box_t::~Box_t(){}

bool Box_t::intersection(const Ray_t &ray, double &t) const {
    const Vec3f invdir = Vec3f(1.0)/ray.direction_;
    const bool sign[3] = {invdir[0] < 0,
                            invdir[1] < 0,
                            invdir[2] < 0};

    double tmin = (coordinates_[sign[0]][0] - ray.origin_[0]) * invdir[0];
    double tmax = (coordinates_[!sign[0]][0] - ray.origin_[0]) * invdir[0];
    const double tymin = (coordinates_[sign[1]][1] - ray.origin_[1]) * invdir[1];
    const double tymax = (coordinates_[!sign[1]][1] - ray.origin_[1]) * invdir[1];

    if ((tmin > tymax) || (tymin > tmax)){
        t = std::numeric_limits<double>::infinity();
        return false;
    }

    tmin = std::max(tmin, tymin);
    tmax = std::min(tmax, tymax);

    const double tzmin = (coordinates_[sign[2]][2] - ray.origin_[2]) * invdir[2];
    const double tzmax = (coordinates_[!sign[2]][2] - ray.origin_[2]) * invdir[2];

    if ((tmin > tzmax) || (tzmin > tmax)){
        t = std::numeric_limits<double>::infinity();
        return false;
    }
    
    t = std::max(tmin, tzmin);
    return true;
}