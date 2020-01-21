#include "Sphere_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <cmath>
#include <limits>

#define PI 3.141592653589793238463

Sphere_t::Sphere_t(Material_t *material, TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix){
    origin_ = transformation_->multVec(Vec3f());
    radius_ = transformation_->getScale(); 
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    const Vec3f direction = transform_norm.multDir(Vec3f(0.0, 0.0, 1.0)).to_sph(); 
    const Vec3f direction2 = transform_norm.multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_ = Vec3f(1.0, direction[1], direction2[2]);
}

Sphere_t::~Sphere_t(){}

void Sphere_t::update(){
    origin_ = transformation_->multVec(Vec3f());
    radius_ = transformation_->getScale();
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    const Vec3f direction = transform_norm.multDir(Vec3f(0.0, 0.0, 1.0)).to_sph(); 
    const Vec3f direction2 = transform_norm.multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_ = Vec3f(1.0, direction[1], direction2[2]);
}

bool Sphere_t::intersection(const Ray_t &ray, double &t, double (&uv)[2]) const {
    const Vec3f to_center = origin_ - ray.origin_;
    const double b = to_center.dot(ray.direction_);
    const double c = to_center.dot(to_center) - pow(radius_, 2);
    const double discriminant = pow(b, 2) - c;

    if (discriminant < 0.0){
        t = std::numeric_limits<double>::infinity();
        uv[0] = NAN;
        uv[1] = NAN;
        return false;
    }
    t = b - sqrt(discriminant);

    if (t < 0.0){
        t = b + sqrt(discriminant);
        if (t < 0.0){
            t = std::numeric_limits<double>::infinity();
            uv[0] = NAN;
            uv[1] = NAN;
            return false; 
        }
    }

    const Vec3f sph = (ray.direction_ * t - to_center).to_sph();
    uv[0] = sph[2]/(2.0 * PI) + 0.5;
    uv[1] = 1.0 - sph[1]/PI;
    return true;
}

void Sphere_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {
    Vec3f sph = Vec3f(1.0, (1.0 - uv[1]) * PI, (uv[0] - 0.5) * 2.0 * PI);
    normalvec = sph.get_xyz();

    sph -= direction_sph_;

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

void Sphere_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {
    normalvec = Vec3f(1.0, (1.0 - uv[1]) * PI, (uv[0] - 0.5) * 2.0 * PI).to_xyz();
}

void Sphere_t::normal_uv_tangent(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const {
    Vec3f sph = Vec3f(1.0, (1.0 - uv[1]) * PI, (uv[0] - 0.5) * 2.0 * PI);
    normalvec = sph.get_xyz();

    sph -= direction_sph_;

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

    tangentvec = direction_sph_.get_xyz().cross(normalvec).normalize_inplace();
} 

Vec3f Sphere_t::mincoord() const {
    return origin_ - radius_;
}

Vec3f Sphere_t::maxcoord() const {
    return origin_ + radius_;
}