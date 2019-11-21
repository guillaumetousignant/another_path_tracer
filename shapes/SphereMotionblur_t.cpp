#include "SphereMotionblur_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <cmath>
#include <limits>
#include "Slerp.h"

#define PI 3.141592653589793238463

SphereMotionblur_t::SphereMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix){
    origin_ = transformation_->multVec(Vec3f());
    origin_last_ = origin_;
    radius_ = transformation_->getScale(); 
    radius_last_ = radius_;
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    const Vec3f direction = transform_norm.multDir(Vec3f(0.0, 0.0, 1.0)).to_sph(); 
    const Vec3f direction2 = transform_norm.multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_ = Vec3f(1.0, direction[1], direction2[2]);
    direction_sph_last_ = direction_sph_;
}

SphereMotionblur_t::~SphereMotionblur_t(){}

void SphereMotionblur_t::update(){
    radius_last_ = radius_;
    origin_last_ = origin_;
    direction_sph_last_ = direction_sph_;

    origin_ = transformation_->multVec(Vec3f());
    radius_ = transformation_->getScale();
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    const Vec3f direction = transform_norm.multDir(Vec3f(0.0, 0.0, 1.0)).to_sph(); 
    const Vec3f direction2 = transform_norm.multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_ = Vec3f(1.0, direction[1], direction2[2]);
}

void SphereMotionblur_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {
    const Vec3f origin_int = origin_ * ray.time_ + origin_last_ * (1.0 - ray.time_);
    const double radius_int = radius_ * ray.time_ + radius_last_ * (1.0 - ray.time_);                   

    const Vec3f to_center = origin_int - ray.origin_;
    const double b = to_center.dot(ray.direction_);
    const double c = to_center.dot(to_center) - pow(radius_int, 2);
    const double discriminant = pow(b, 2) - c;

    if (discriminant < 0.0){
        intersected = false;
        t = std::numeric_limits<double>::infinity();
        uv[0] = NAN;
        uv[1] = NAN;
        return;
    }
    else{
        t = b - sqrt(discriminant);
    }

    if (t < 0.0){
        t = b + sqrt(discriminant);
        if (t < 0.0){
            intersected = false;
            t = std::numeric_limits<double>::infinity();
            uv[0] = NAN;
            uv[1] = NAN;
            return; 
        }
    }

    intersected = true;
    const Vec3f sph = (ray.direction_ * t - to_center).to_sph();
    uv[0] = sph[2]/(2.0 * PI) + 0.5;
    uv[1] = 1.0 - sph[1]/PI;
}

void SphereMotionblur_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {
    Vec3f sph = Vec3f(1.0, (1.0 - uv[1]) * PI, (uv[0] - 0.5) * 2.0 * PI);
    normalvec = sph.get_xyz();

    const Vec3f offset = Vec3f(1.0, direction_sph_[1] * ray.time_ + direction_sph_last_[1] * (1.0 - ray.time_),
                        slerp(direction_sph_[2], direction_sph_last_[2], ray.time_));

    sph -= offset;

    // CHECK change
    if (sph[1] < 0.0){
        sph[1] = -sph[1];
        sph[2] += PI;
    }
    else if (sph[1] > PI){
        sph[1] = 2.0*PI - sph[1];
        sph[2] += PI;
    }

    // CHECK change
    if (sph[2] < -PI){
        sph[2] += 2.0*PI;
    }
    else if (sph[2] > PI){
        sph[2] -= 2.0*PI;
    }

    tuv[0] = sph[2]/(2.0 * PI) + 0.5;
    tuv[1] = 1.0 - sph[1]/PI;
}

void SphereMotionblur_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {
    normalvec = Vec3f(1.0, (1.0 - uv[1]) * PI, (uv[0] - 0.5) * 2.0 * PI).to_xyz();
}

void SphereMotionblur_t::normal_uv_tangent(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const {
    Vec3f sph = Vec3f(1.0, (1.0 - uv[1]) * PI, (uv[0] - 0.5) * 2.0 * PI);
    normalvec = sph.get_xyz();

    const Vec3f direction_int = direction_sph_.get_xyz() * ray.time_ + direction_sph_last_.get_xyz() * (1.0 - ray.time_);

    const Vec3f offset = Vec3f(1.0, direction_sph_[1] * ray.time_ + direction_sph_last_[1] * (1.0 - ray.time_),
                        slerp(direction_sph_[2], direction_sph_last_[2], ray.time_));

    sph -= offset;

    // CHECK change
    if (sph[1] < 0.0){
        sph[1] = -sph[1];
        sph[2] += PI;
    }
    else if (sph[1] > PI){
        sph[1] = 2.0*PI - sph[1];
        sph[2] += PI;
    }

    // CHECK change
    if (sph[2] < -PI){
        sph[2] += 2.0*PI;
    }
    else if (sph[2] > PI){
        sph[2] -= 2.0*PI;
    }

    tuv[0] = sph[2]/(2.0 * PI) + 0.5;
    tuv[1] = 1.0 - sph[1]/PI;

    tangentvec = direction_int.cross(normalvec).normalize();
} 

Vec3f SphereMotionblur_t::mincoord() const {
    return (origin_ - radius_).getMin(origin_last_ - radius_last_);
}

Vec3f SphereMotionblur_t::maxcoord() const {
    return (origin_ + radius_).getMax(origin_last_ + radius_last_);
}