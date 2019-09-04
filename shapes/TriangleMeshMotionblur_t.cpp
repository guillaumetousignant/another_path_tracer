#include "TriangleMeshMotionblur_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include "MeshGeometry_t.h"

#define PI 3.141592653589793238463
#define EPSILON 0.00000001

TriangleMeshMotionblur_t::TriangleMeshMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index) 
    : Shape_t(material, transform_matrix), geom_(geom), index_(index) {

    const TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_[i] = transformation_->multVec(geom_->v_[3 * index_ + i]);
        points_last_[i] = points_[i];
        normals_[i] = transform_norm.multDir(geom_->vn_[3 * index_ + i]); // was transformation_
        normals_last_[i] = normals_[i];
    }

    v0v1_ = points_[1] - points_[0];
    v0v1_last_ = v0v1_;
    v0v2_ = points_[2] - points_[0];
    v0v2_last_ = v0v2_;
}

TriangleMeshMotionblur_t::~TriangleMeshMotionblur_t(){}

void TriangleMeshMotionblur_t::update() {
    const TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_last_[i] = points_[i];
        normals_last_[i] = normals_[i];
        points_[i] = transformation_->multVec(geom_->v_[3 * index_ + i]);
        normals_[i] = transform_norm.multDir(geom_->vn_[3 * index_ + i]); // was transformation_
    }

    v0v1_last_ = v0v1_;
    v0v2_last_ = v0v2_;
    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];
}

void TriangleMeshMotionblur_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {
const Vec3f v0v1_int = v0v1_ * ray.time_ + v0v1_last_ * (1.0 - ray.time_);
    const Vec3f v0v2_int = v0v2_ * ray.time_ + v0v2_last_ * (1.0 - ray.time_);
    const Vec3f points_int[3] = {points_[0] * ray.time_ + points_last_[0] * (1.0 - ray.time_),
                                    points_[1] * ray.time_ + points_last_[1] * (1.0 - ray.time_),
                                    points_[2] * ray.time_ + points_last_[2] * (1.0 - ray.time_)};

    const Vec3f pvec = ray.direction_.cross(v0v2_int);
    const double det = v0v1_int.dot(pvec);

    if (std::abs(det) < EPSILON){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        uv[0] = NAN;
        uv[1] = NAN;
        return;
    }

    const double invdet = 1.0/det;
    const Vec3f tvec = ray.origin_ - points_int[0];
    const double u = tvec.dot(pvec) * invdet;
    uv[0] = u;

    if ((u < 0.0) || (u > 1.0)){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        uv[1] = NAN;
        return;
    }

    const Vec3f qvec = tvec.cross(v0v1_int);
    const double v = ray.direction_.dot(qvec) * invdet;
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

void TriangleMeshMotionblur_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {
    const Vec3f normals_int[3] = {normals_[0] * ray.time_ + normals_last_[0] * (1.0 - ray.time_),
                                    normals_[1] * ray.time_ + normals_last_[1] * (1.0 - ray.time_),
                                    normals_[2] * ray.time_ + normals_last_[2] * (1.0 - ray.time_)};

    const Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_int[0][0] + distance[1] * normals_int[1][0] + distance[2] * normals_int[2][0], 
        distance[0] * normals_int[0][1] + distance[1] * normals_int[1][1] + distance[2] * normals_int[2][1],
        distance[0] * normals_int[0][2] + distance[1] * normals_int[1][2] + distance[2] * normals_int[2][2]);
    // Matrix multiplication, optimise.
    tuv[0] = distance[0] * geom_->vt_[3*index_][0] + distance[1] * geom_->vt_[3*index_ + 1][0] + distance[2] * geom_->vt_[3*index_ + 2][0];
    tuv[1] = distance[0] * geom_->vt_[3*index_][1] + distance[1] * geom_->vt_[3*index_ + 1][1] + distance[2] * geom_->vt_[3*index_ + 2][1];
}

void TriangleMeshMotionblur_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {
    const Vec3f normals_int[3] = {normals_[0] * ray.time_ + normals_last_[0] * (1.0 - ray.time_),
                                    normals_[1] * ray.time_ + normals_last_[1] * (1.0 - ray.time_),
                                    normals_[2] * ray.time_ + normals_last_[2] * (1.0 - ray.time_)};

    const Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_int[0][0] + distance[1] * normals_int[1][0] + distance[2] * normals_int[2][0], 
        distance[0] * normals_int[0][1] + distance[1] * normals_int[1][1] + distance[2] * normals_int[2][1],
        distance[0] * normals_int[0][2] + distance[1] * normals_int[1][2] + distance[2] * normals_int[2][2]);
    // Matrix multiplication, optimise.
}

void TriangleMeshMotionblur_t::normal_face(const Ray_t &ray, Vec3f &normalvec) const{
    const Vec3f v0v1_int = v0v1_ * ray.time_ + v0v1_last_ * (1.0 - ray.time_);
    const Vec3f v0v2_int = v0v2_ * ray.time_ + v0v2_last_ * (1.0 - ray.time_);

    normalvec = v0v1_int.cross(v0v2_int).normalize();
}

Vec3f TriangleMeshMotionblur_t::mincoord() const {
    return points_[0].getMin(points_[1]).min(points_[2]).min(points_last_[0]).min(points_last_[1]).min(points_last_[2]);
}

Vec3f TriangleMeshMotionblur_t::maxcoord() const {
    return points_[0].getMax(points_[1]).max(points_[2]).max(points_last_[0]).max(points_last_[1]).max(points_last_[2]);
}