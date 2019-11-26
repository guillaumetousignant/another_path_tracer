#include "TriangleMesh_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include "MeshGeometry_t.h"

#define PI 3.141592653589793238463
#define EPSILON 0.00000001

TriangleMesh_t::TriangleMesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index, unsigned int id) 
    : Shape_t(material, transform_matrix, id), geom_(geom), index_(index) {

    const TransformMatrix_t transform_norm = transformation_->transformDir();

    points_[0] = transformation_->multVec(geom_->v_[3 * index_]);
    points_[1] = transformation_->multVec(geom_->v_[3 * index_ + 1]);
    points_[2] = transformation_->multVec(geom_->v_[3 * index_ + 2]);
    normals_[0] = transform_norm.multDir(geom_->vn_[3 * index_]);
    normals_[1] = transform_norm.multDir(geom_->vn_[3 * index_ + 1]);
    normals_[2] = transform_norm.multDir(geom_->vn_[3 * index_ + 2]);

    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];

    const double tuv0v1[2] = {geom->vt_[3 * index_ + 1][0] - geom->vt_[3 * index_][0], geom->vt_[3 * index_ + 1][1] - geom->vt_[3 * index_][1]};
    const double tuv0v2[2] = {geom->vt_[3 * index_ + 2][0] - geom->vt_[3 * index_][0], geom->vt_[3 * index_ + 2][1] - geom->vt_[3 * index_][1]};    

    const double invdet = 1.0/(tuv0v1[0] * tuv0v2[1] - tuv0v1[1] * tuv0v2[0]);
    if (std::isfinite(invdet)){
        tuv_to_world_[0] = invdet * -tuv0v2[0];
        tuv_to_world_[1] = invdet * tuv0v1[0];
    }
    else {
        tuv_to_world_[0] = 1.0;
        tuv_to_world_[1] = 0.0;
    }
    tangent_vec_ = v0v1_ * tuv_to_world_[0] + v0v2_ * tuv_to_world_[1];
}

TriangleMesh_t::~TriangleMesh_t(){}

void TriangleMesh_t::update() {
    const TransformMatrix_t transform_norm = transformation_->transformDir();

    points_[0] = transformation_->multVec(geom_->v_[3 * index_]);
    points_[1] = transformation_->multVec(geom_->v_[3 * index_ + 1]);
    points_[2] = transformation_->multVec(geom_->v_[3 * index_ + 2]);
    normals_[0] = transform_norm.multDir(geom_->vn_[3 * index_]);
    normals_[1] = transform_norm.multDir(geom_->vn_[3 * index_ + 1]);
    normals_[2] = transform_norm.multDir(geom_->vn_[3 * index_ + 2]);

    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];

    tangent_vec_ = v0v1_ * tuv_to_world_[0] + v0v2_ * tuv_to_world_[1];
}

void TriangleMesh_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {
    const Vec3f pvec = ray.direction_.cross(v0v2_);
    const double det = v0v1_.dot(pvec);

    if (std::abs(det) < EPSILON){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        uv[0] = NAN;
        uv[1] = NAN;
        return;
    }

    const double invdet = 1.0/det;
    const Vec3f tvec = ray.origin_ - points_[0];
    const double u = tvec.dot(pvec) * invdet;
    uv[0] = u;

    if ((u < 0.0) || (u > 1.0)){
        t = std::numeric_limits<double>::infinity();
        intersected = false;
        uv[1] = NAN;
        return;
    }

    const Vec3f qvec = tvec.cross(v0v1_);
    const double v = ray.direction_.dot(qvec) * invdet;
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
    const Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_[0][0] + distance[1] * normals_[1][0] + distance[2] * normals_[2][0], 
        distance[0] * normals_[0][1] + distance[1] * normals_[1][1] + distance[2] * normals_[2][1],
        distance[0] * normals_[0][2] + distance[1] * normals_[1][2] + distance[2] * normals_[2][2]);
    // Matrix multiplication, optimise.
    tuv[0] = distance[0] * geom_->vt_[3*index_][0] + distance[1] * geom_->vt_[3*index_ + 1][0] + distance[2] * geom_->vt_[3*index_ + 2][0];
    tuv[1] = distance[0] * geom_->vt_[3*index_][1] + distance[1] * geom_->vt_[3*index_ + 1][1] + distance[2] * geom_->vt_[3*index_ + 2][1];
}

void TriangleMesh_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {
    const Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_[0][0] + distance[1] * normals_[1][0] + distance[2] * normals_[2][0], 
        distance[0] * normals_[0][1] + distance[1] * normals_[1][1] + distance[2] * normals_[2][1],
        distance[0] * normals_[0][2] + distance[1] * normals_[1][2] + distance[2] * normals_[2][2]);
    // Matrix multiplication, optimise.
}

void TriangleMesh_t::normal_uv_tangent(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const {
    const Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_[0][0] + distance[1] * normals_[1][0] + distance[2] * normals_[2][0], 
        distance[0] * normals_[0][1] + distance[1] * normals_[1][1] + distance[2] * normals_[2][1],
        distance[0] * normals_[0][2] + distance[1] * normals_[1][2] + distance[2] * normals_[2][2]);
    // Matrix multiplication, optimise.
    tuv[0] = distance[0] * geom_->vt_[3*index_][0] + distance[1] * geom_->vt_[3*index_ + 1][0] + distance[2] * geom_->vt_[3*index_ + 2][0];
    tuv[1] = distance[0] * geom_->vt_[3*index_][1] + distance[1] * geom_->vt_[3*index_ + 1][1] + distance[2] * geom_->vt_[3*index_ + 2][1];

    tangentvec = tangent_vec_.cross(normalvec).normalize();
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