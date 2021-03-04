#include "shapes/SphereMotionblur_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Material_t.h"
#include <cmath>
#include <limits>
#include "functions/Slerp.h"

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Shapes::SphereMotionblur_t::SphereMotionblur_t(APTracer::Entities::Material_t *material, APTracer::Entities::TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix) {
    origin_ = transformation_->multVec(Vec3f());
    origin_last_ = origin_;
    radius_ = transformation_->getScale(); 
    radius_last_ = radius_;
    const Vec3f direction = transformation_->multDir(Vec3f(0.0, 0.0, 1.0)).to_sph(); 
    const Vec3f direction2 = transformation_->multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_ = Vec3f(1.0, direction[1], direction2[2]);
    direction_sph_last_ = direction_sph_;
}

APTracer::Shapes::SphereMotionblur_t::~SphereMotionblur_t() = default;

void APTracer::Shapes::SphereMotionblur_t::update() {
    radius_last_ = radius_;
    origin_last_ = origin_;
    direction_sph_last_ = direction_sph_;

    origin_ = transformation_->multVec(Vec3f());
    radius_ = transformation_->getScale();
    const Vec3f direction = transformation_->multDir(Vec3f(0.0, 0.0, 1.0)).to_sph(); 
    const Vec3f direction2 = transformation_->multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_ = Vec3f(1.0, direction[1], direction2[2]);
}

bool APTracer::Shapes::SphereMotionblur_t::intersection(const APTracer::Entities::Ray_t &ray, double &t, std::array<double, 2> &uv) const {
    const Vec3f origin_int = origin_ * ray.time_ + origin_last_ * (1.0 - ray.time_);
    const double radius_int = radius_ * ray.time_ + radius_last_ * (1.0 - ray.time_);                   

    const Vec3f to_center = origin_int - ray.origin_;
    const double b = to_center.dot(ray.direction_);
    const double c = to_center.dot(to_center) - pow(radius_int, 2);
    const double discriminant = pow(b, 2) - c;

    if (discriminant < 0.0) {
        t = std::numeric_limits<double>::infinity();
        uv = {NAN, NAN};
        return false;
    }
    t = b - sqrt(discriminant);

    if (t < 0.0) {
        t = b + sqrt(discriminant);
        if (t < 0.0) {
            t = std::numeric_limits<double>::infinity();
            uv = {NAN, NAN};
            return false; 
        }
    }

    const Vec3f sph = (ray.direction_ * t - to_center).to_sph();
    uv = {sph[2]/(2.0 * pi) + 0.5, 1.0 - sph[1]/pi};
    return true;
}

Vec3f APTracer::Shapes::SphereMotionblur_t::normaluv(double time, std::array<double, 2> uv, std::array<double, 2> &tuv) const {
    Vec3f sph = Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi);
    const Vec3f normalvec = sph.get_xyz();

    const Vec3f offset = Vec3f(1.0, direction_sph_[1] * time + direction_sph_last_[1] * (1.0 - time),
                        slerp(direction_sph_[2], direction_sph_last_[2], time));

    sph -= offset;

    // CHECK change
    if (sph[1] < 0.0) {
        sph[1] = -sph[1];
        sph[2] += pi;
    }
    else if (sph[1] > pi) {
        sph[1] = 2.0*pi - sph[1];
        sph[2] += pi;
    }

    // CHECK change
    if (sph[2] < -pi) {
        sph[2] += 2.0*pi;
    }
    else if (sph[2] > pi) {
        sph[2] -= 2.0*pi;
    }

    tuv = {sph[2]/(2.0 * pi) + 0.5, 1.0 - sph[1]/pi};
    return normalvec;
}

Vec3f APTracer::Shapes::SphereMotionblur_t::normal(double time, std::array<double, 2> uv) const {
    return Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi).to_xyz();
}

Vec3f APTracer::Shapes::SphereMotionblur_t::normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2> &tuv, Vec3f &tangentvec) const {
    Vec3f sph = Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi);
    const Vec3f normalvec = sph.get_xyz();

    const Vec3f direction_int = direction_sph_.get_xyz() * time + direction_sph_last_.get_xyz() * (1.0 - time);

    const Vec3f offset = Vec3f(1.0, direction_sph_[1] * time + direction_sph_last_[1] * (1.0 - time),
                        slerp(direction_sph_[2], direction_sph_last_[2], time));

    sph -= offset;

    // CHECK change
    if (sph[1] < 0.0) {
        sph[1] = -sph[1];
        sph[2] += pi;
    }
    else if (sph[1] > pi) {
        sph[1] = 2.0*pi - sph[1];
        sph[2] += pi;
    }

    // CHECK change
    if (sph[2] < -pi) {
        sph[2] += 2.0*pi;
    }
    else if (sph[2] > pi) {
        sph[2] -= 2.0*pi;
    }

    tuv = {sph[2]/(2.0 * pi) + 0.5, 1.0 - sph[1]/pi};

    tangentvec = direction_int.cross(normalvec).normalize_inplace();
    return normalvec;
} 

Vec3f APTracer::Shapes::SphereMotionblur_t::mincoord() const {
    return (origin_ - radius_).getMin(origin_last_ - radius_last_);
}

Vec3f APTracer::Shapes::SphereMotionblur_t::maxcoord() const {
    return (origin_ + radius_).getMax(origin_last_ + radius_last_);
}