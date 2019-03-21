#include "TriangleMesh_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <math.h>
#include <limits>
#include "Referentials.h"
#include <algorithm>
#include "MeshGeometry_t.h"

#define PI 3.141592653589793238463
#define EPSILON 0.00000001

TriangleMesh_t::TriangleMesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index) 
    : TriangleTop_t(material, transform_matrix), geom_(geom), index_(index) {

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_[i] = transformation_->multVec(geom_->v_[3 * index_ + i]);
    }

    TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        normals_[i] = transformation_->multDir(geom_->vn_[3 * index_ + i]);
    }

    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];
}

TriangleMesh_t::~TriangleMesh_t(){}

void TriangleMesh_t::update() {
    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_[i] = transformation_->multVec(geom_->v_[3 * index_ + i]);
    }

    TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        normals_[i] = transformation_->multDir(geom_->vn_[3 * index_ + i]);
    }

    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];
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