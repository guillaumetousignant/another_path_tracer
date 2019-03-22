#include "Box_t.h"
#include <limits>
#include <algorithm>

Box_t::Box_t(Vec3f (&coord)[2]) : coordinates_{coord[0], coord[1]} {}

Box_t::~Box_t(){}

void Box_t::intersection(const Ray_t &ray, bool &intersected, double &t) const {
    Vec3f invdir;
    bool sign[3];
    double tmin, tmax;
    double tymin, tymax;
    double tzmin, tzmax;

    invdir = Vec3f(1.0, 1.0, 1.0)/ray.direction_;
    sign[0] = invdir[0] < 0;
    sign[1] = invdir[1] < 0;
    sign[2] = invdir[2] < 0;

    tmin = (coordinates_[sign[0]][0] - ray.origin_[0]) * invdir[0];
    tmax = (coordinates_[!sign[0]][0] - ray.origin_[0]) * invdir[0];
    tymin = (coordinates_[sign[1]][1] - ray.origin_[1]) * invdir[1];
    tymax = (coordinates_[!sign[1]][1] - ray.origin_[1]) * invdir[1];

    if ((tmin > tymax) || (tymin > tmax)){
        intersected = false;
        t = std::numeric_limits<double>::infinity();
        return;
    }

    tmin = std::max(tmin, tymin);
    tmax = std::min(tmax, tymax);

    tzmin = (coordinates_[sign[2]][2] - ray.origin_[2]) * invdir[2];
    tzmax = (coordinates_[!sign[2]][2] - ray.origin_[2]) * invdir[2];

    if ((tmin > tzmax) || (tzmin > tmax)){
        intersected = false;
        t = std::numeric_limits<double>::infinity();
        return;
    }
    
    t = std::max(tmin, tzmin);
    intersected = true;
}