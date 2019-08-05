#include "TriangleMesh_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include "MeshGeometry_t.h"

#define PI 3.141592653589793238463
#define EPSILON 0.00000001

TriangleMesh_t::TriangleMesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index) 
    : Shape_t(material, transform_matrix), geom_(geom), index_(index) {

    TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_[i] = transformation_->multVec(geom_->v_[3 * index_ + i]);
        normals_[i] = transform_norm.multDir(geom_->vn_[3 * index_ + i]); // was transformation_
    }

    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];
}

TriangleMesh_t::~TriangleMesh_t(){}

void TriangleMesh_t::update() {
    TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_[i] = transformation_->multVec(geom_->v_[3 * index_ + i]);
        normals_[i] = transform_norm.multDir(geom_->vn_[3 * index_ + i]); // was transformation_
    }

    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];
}

void TriangleMesh_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {
    Vec3f pvec, tvec, qvec;
    double det, invdet;
    double u, v;

    pvec = ray.direction_.cross(v0v2_);
    det = v0v1_.dot(pvec);

    if (std::abs(det) < EPSILON){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        uv[0] = NAN;
        uv[1] = NAN;
        return;
    }

    invdet = 1.0/det;
    tvec = ray.origin_ - points_[0];
    u = tvec.dot(pvec) * invdet;
    uv[0] = u;

    if ((u < 0.0) || (u > 1.0)){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        uv[1] = NAN;
        return;
    }

    qvec = tvec.cross(v0v1_);
    v = ray.direction_.dot(qvec) * invdet;
    uv[1] = v;

    if ((v < 0.0) || ((u+v) > 1.0)){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        return;
    }

    t = v0v2_.dot(qvec) * invdet;

    if (t < 0.0){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        return;
    }

    intersected = true;
}

void TriangleMesh_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {
    Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_[0][0] + distance[1] * normals_[1][0] + distance[2] * normals_[2][0], 
        distance[0] * normals_[0][1] + distance[1] * normals_[1][1] + distance[2] * normals_[2][1],
        distance[0] * normals_[0][2] + distance[1] * normals_[1][2] + distance[2] * normals_[2][2]);
    // Matrix multiplication, optimise.
    tuv[0] = distance[0] * geom_->vt_[3*index_][0] + distance[1] * geom_->vt_[3*index_ + 1][0] + distance[2] * geom_->vt_[3*index_ + 2][0];
    tuv[1] = distance[0] * geom_->vt_[3*index_][1] + distance[1] * geom_->vt_[3*index_ + 1][1] + distance[2] * geom_->vt_[3*index_ + 2][1];
}

void TriangleMesh_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {
    Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_[0][0] + distance[1] * normals_[1][0] + distance[2] * normals_[2][0], 
        distance[0] * normals_[0][1] + distance[1] * normals_[1][1] + distance[2] * normals_[2][1],
        distance[0] * normals_[0][2] + distance[1] * normals_[1][2] + distance[2] * normals_[2][2]);
    // Matrix multiplication, optimise.
}

void TriangleMesh_t::normal_face(const Ray_t &ray, Vec3f &normalvec) const{
    normalvec = v0v1_.cross(v0v2_).normalize();
}

Vec3f TriangleMesh_t::mincoord() const {
    return points_[0].getMin(points_[1]).min(points_[2]);
}

Vec3f TriangleMesh_t::maxcoord() const {
    return points_[0].getMax(points_[1]).max(points_[2]);
}