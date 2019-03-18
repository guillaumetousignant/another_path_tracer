#include "Triangle_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <math.h>
#include <limits>
#include "Referentials.h"
#include <algorithm>

#define PI 3.141592653589793238463
#define EPSILON 0.00000001

Triangle_t::Triangle_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f (&points)[3], Vec3f (&normals)[3], double (&texcoord)[3][2]) 
    : TriangleTop_t(material, transform_matrix), points_orig_{points[0], points[1], points[2]} {
    v0v1_ = points[1] - points[0];
    v0v2_ = points[2] - points[0];
    
    

}

Triangle_t::~Triangle_t(){}

void Triangle_t::update() {

}

void Triangle_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {

}