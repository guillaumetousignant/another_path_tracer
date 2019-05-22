#include "TriangleMotionblur_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <cmath>
#include <limits>
#include <algorithm>

#define PI 3.141592653589793238463
#define EPSILON 0.00000001

TriangleMotionblur_t::TriangleMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f* points, Vec3f* normals, double** texcoord) 
    : Triangle_t(material, transform_matrix, points, normals, texcoord),
    points_last_{points_[0], points_[1], points_[2]}, normals_last_{normals_[0], normals_[1], normals_[2]}, 
    v0v1_last_(v0v1_), v0v2_last_(v0v2_) {}

TriangleMotionblur_t::~TriangleMotionblur_t(){}

void TriangleMotionblur_t::update() {    
    TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_last_[i] = points_[i];
        normals_last_[i] = normals_[i];
        points_[i] = transformation_->multVec(points_orig_[i]);
        normals_[i] = transform_norm.multDir(normals_orig_[i]);
    }

    v0v1_last_ = v0v1_;
    v0v2_last_ = v0v2_;
    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];
}

void TriangleMotionblur_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {
    Vec3f pvec, tvec, qvec;
    double det, invdet;
    double u, v;
    Vec3f v0v1_int, v0v2_int;
    Vec3f points_int[3];

    v0v1_int = v0v1_ * ray.time_ + v0v1_last_ * (1.0 - ray.time_);
    v0v2_int = v0v2_ * ray.time_ + v0v2_last_ * (1.0 - ray.time_);
    for (unsigned int i = 0; i < 3; i++){
        points_int[i] = points_[i] * ray.time_ + points_last_[i] * (1.0 - ray.time_);
    }

    pvec = ray.direction_.cross(v0v2_int);
    det = v0v1_int.dot(pvec);

    if (std::abs(det) < EPSILON){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        uv[0] = NAN;
        uv[1] = NAN;
        return;
    }

    invdet = 1.0/det;
    tvec = ray.origin_ - points_int[0];
    u = tvec.dot(pvec) * invdet;
    uv[0] = u;

    if ((u < 0.0) || (u > 1.0)){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        uv[1] = NAN;
        return;
    }

    qvec = tvec.cross(v0v1_int);
    v = ray.direction_.dot(qvec) * invdet;
    uv[1] = v;

    if ((v < 0.0) || ((u+v) > 1.0)){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        return;
    }

    t = v0v2_int.dot(qvec) * invdet;

    if (t < 0.0){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        return;
    }

    intersected = true;
}

void TriangleMotionblur_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {
    Vec3f normals_int[3];
    for (unsigned int i = 0; i < 3; i++){
        normals_int[i] = normals_[i] * ray.time_ + normals_last_[i] * (1.0 - ray.time_);
    }

    Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_int[0][0] + distance[1] * normals_int[1][0] + distance[2] * normals_int[2][0], 
        distance[0] * normals_int[0][1] + distance[1] * normals_int[1][1] + distance[2] * normals_int[2][1],
        distance[0] * normals_int[0][2] + distance[1] * normals_int[1][2] + distance[2] * normals_int[2][2]);
    // Matrix multiplication, optimise.
    tuv[0] = distance[0] * texture_coordinates_[0][0] + distance[1] * texture_coordinates_[1][0] + distance[2] * texture_coordinates_[2][0];
    tuv[1] = distance[0] * texture_coordinates_[0][1] + distance[1] * texture_coordinates_[1][1] + distance[2] * texture_coordinates_[2][1];
}

void TriangleMotionblur_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {
    Vec3f normals_int[3];
    for (unsigned int i = 0; i < 3; i++){
        normals_int[i] = normals_[i] * ray.time_ + normals_last_[i] * (1.0 - ray.time_);
    }

    Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_int[0][0] + distance[1] * normals_int[1][0] + distance[2] * normals_int[2][0], 
        distance[0] * normals_int[0][1] + distance[1] * normals_int[1][1] + distance[2] * normals_int[2][1],
        distance[0] * normals_int[0][2] + distance[1] * normals_int[1][2] + distance[2] * normals_int[2][2]);
    // Matrix multiplication, optimise.
}

void TriangleMotionblur_t::normal_face(const Ray_t &ray, Vec3f &normalvec) const{
    Vec3f v0v1_int, v0v2_int;

    v0v1_int = v0v1_ * ray.time_ + v0v1_last_ * (1.0 - ray.time_);
    v0v2_int = v0v2_ * ray.time_ + v0v2_last_ * (1.0 - ray.time_);

    normalvec = v0v1_int.cross(v0v2_int).normalize();
}

Vec3f TriangleMotionblur_t::mincoord() const {
    return points_[0].getMin(points_[1]).min(points_[2]).min(points_last_[0]).min(points_last_[1]).min(points_last_[2]);
}

Vec3f TriangleMotionblur_t::maxcoord() const {
    return points_[0].getMax(points_[1]).max(points_[2]).max(points_last_[0]).max(points_last_[1]).max(points_last_[2]);
}