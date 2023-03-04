#include "shapes/Box_t.hpp"
#include <algorithm>
#include <array>

using APTracer::Entities::Vec3f;

APTracer::Shapes::Box_t::Box_t() : coordinates_{Vec3f(), Vec3f()} {}

APTracer::Shapes::Box_t::Box_t(std::array<Vec3f, 2> coord) : coordinates_(coord) {}

auto APTracer::Shapes::Box_t::intersection(const APTracer::Entities::Ray_t& ray, double& t) const -> bool {
    const Vec3f invdir = Vec3f(1.0) / ray.direction_;
    const std::array<bool, 3> sign{invdir[0] < 0, invdir[1] < 0, invdir[2] < 0};

    double tmin        = (coordinates_[static_cast<size_t>(sign[0])][0] - ray.origin_[0]) * invdir[0];
    double tmax        = (coordinates_[static_cast<size_t>(!sign[0])][0] - ray.origin_[0]) * invdir[0];
    const double tymin = (coordinates_[static_cast<size_t>(sign[1])][1] - ray.origin_[1]) * invdir[1];
    const double tymax = (coordinates_[static_cast<size_t>(!sign[1])][1] - ray.origin_[1]) * invdir[1];

    if ((tmin > tymax) || (tymin > tmax)) {
        return false;
    }

    tmin = std::max(tmin, tymin);
    tmax = std::min(tmax, tymax);

    const double tzmin = (coordinates_[static_cast<size_t>(sign[2])][2] - ray.origin_[2]) * invdir[2];
    const double tzmax = (coordinates_[static_cast<size_t>(!sign[2])][2] - ray.origin_[2]) * invdir[2];

    if ((tmin > tzmax) || (tzmin > tmax)) {
        return false;
    }

    t = std::max(tmin, tzmin);
    return true;
}
