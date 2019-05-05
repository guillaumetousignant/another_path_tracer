#include "Triangle_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <math.h>
#include <limits>
#include "Referentials.h"
#include <algorithm>

#define PI 3.141592653589793238463
#define EPSILON 0.00000001

Triangle_t::Triangle_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f* points, Vec3f* normals, double** texcoord) 
    : TriangleTop_t(material, transform_matrix), points_orig_{points[0], points[1], points[2]} {

    if (normals == nullptr){
        Vec3f nor = (points[1] - points[0]).cross(points[2] - points[0]).normalize(); 
        // Loop or explicit?
        for (unsigned int i = 0; i < 3; i++){
            normals_orig_[i] = nor;
        }
    }
    else{
        for (unsigned int i = 0; i < 3; i++){
            normals_orig_[i] = normals[i];
        }
    }

    if (texcoord == nullptr){
        for (unsigned int i = 0; i < 3; i++){
            for (unsigned int j = 0; j < 2; j++){
                texture_coordinates_[i][j] = 0;
            }
        }
    }
    else{
        for (unsigned int i = 0; i < 3; i++){
            for (unsigned int j = 0; j < 2; j++){
                texture_coordinates_[i][j] = texcoord[i][j];
            }
        }
    }

    TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_[i] = transformation_->multVec(points_orig_[i]);
        normals_[i] = transform_norm.multDir(normals_orig_[i]); // was transformation_ before
    }

    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];
}

Triangle_t::~Triangle_t(){}

void Triangle_t::update() {
    TransformMatrix_t transform_norm = transformation_->transformDir();

    for (unsigned int i = 0; i < 3; i++){ // Loop or explicit?
        points_[i] = transformation_->multVec(points_orig_[i]);
        normals_[i] = transform_norm.multDir(normals_orig_[i]);
    }

    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];
}

void Triangle_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {
    Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    normalvec = Vec3f(distance[0] * normals_[0][0] + distance[1] * normals_[1][0] + distance[2] * normals_[2][0], 
        distance[0] * normals_[0][1] + distance[1] * normals_[1][1] + distance[2] * normals_[2][1],
        distance[0] * normals_[0][2] + distance[1] * normals_[1][2] + distance[2] * normals_[2][2]);
    // Matrix multiplication, optimise.
    tuv[0] = distance[0] * texture_coordinates_[0][0] + distance[1] * texture_coordinates_[1][0] + distance[2] * texture_coordinates_[2][0];
    tuv[1] = distance[0] * texture_coordinates_[0][1] + distance[1] * texture_coordinates_[1][1] + distance[2] * texture_coordinates_[2][1];
}