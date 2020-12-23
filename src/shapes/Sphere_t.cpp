#include "shapes/Sphere_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Material_t.h"
#include <cmath>
#include <limits>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Shapes::Sphere_t::Sphere_t(APTracer::Entities::Material_t *material, APTracer::Entities::TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix) {
    origin_ = transformation_->multVec(Vec3f());
    radius_ = transformation_->getScale(); 
    const APTracer::Entities::TransformMatrix_t transform_norm = transformation_->transformDir();
    const Vec3f direction = transform_norm.multDir(Vec3f(0.0, 0.0, 1.0)).to_sph(); 
    const Vec3f direction2 = transform_norm.multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_ = Vec3f(1.0, direction[1], direction2[2]);
}

APTracer::Shapes::Sphere_t::~Sphere_t() {}

void APTracer::Shapes::Sphere_t::update() {
    origin_ = transformation_->multVec(Vec3f());
    radius_ = transformation_->getScale();
    const APTracer::Entities::TransformMatrix_t transform_norm = transformation_->transformDir();
    const Vec3f direction = transform_norm.multDir(Vec3f(0.0, 0.0, 1.0)).to_sph(); 
    const Vec3f direction2 = transform_norm.multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_ = Vec3f(1.0, direction[1], direction2[2]);
}

bool APTracer::Shapes::Sphere_t::intersection(const APTracer::Entities::Ray_t &ray, double &t, double (&uv)[2]) const {
    const Vec3f to_center = origin_ - ray.origin_;
    const double b = to_center.dot(ray.direction_);
    const double c = to_center.dot(to_center) - pow(radius_, 2);
    const double discriminant = pow(b, 2) - c;

    if (discriminant < 0.0) {
        t = std::numeric_limits<double>::infinity();
        uv[0] = NAN;
        uv[1] = NAN;
        return false;
    }
    t = b - sqrt(discriminant);

    if (t < 0.0) {
        t = b + sqrt(discriminant);
        if (t < 0.0) {
            t = std::numeric_limits<double>::infinity();
            uv[0] = NAN;
            uv[1] = NAN;
            return false; 
        }
    }

    const Vec3f sph = (ray.direction_ * t - to_center).to_sph();
    uv[0] = sph[2]/(2.0 * pi) + 0.5;
    uv[1] = 1.0 - sph[1]/pi;
    return true;
}

Vec3f APTracer::Shapes::Sphere_t::normaluv(double time, const double (&uv)[2], double (&tuv)[2]) const {
    Vec3f sph = Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi);
    const Vec3f normalvec = sph.get_xyz();

    sph -= direction_sph_;

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

    tuv[0] = sph[2]/(2.0 * pi) + 0.5;
    tuv[1] = 1.0 - sph[1]/pi;
    return normalvec;
}

Vec3f APTracer::Shapes::Sphere_t::normal(double time, const double (&uv)[2]) const {
    return Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi).to_xyz();
}

Vec3f APTracer::Shapes::Sphere_t::normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &tangentvec) const {
    Vec3f sph = Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi);
    const Vec3f normalvec = sph.get_xyz();

    sph -= direction_sph_;

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

    tuv[0] = sph[2]/(2.0 * pi) + 0.5;
    tuv[1] = 1.0 - sph[1]/pi;

    tangentvec = direction_sph_.get_xyz().cross(normalvec).normalize_inplace();
    return normalvec;
} 

Vec3f APTracer::Shapes::Sphere_t::mincoord() const {
    return origin_ - radius_;
}

Vec3f APTracer::Shapes::Sphere_t::maxcoord() const {
    return origin_ + radius_;
}