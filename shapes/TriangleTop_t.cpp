#include "TriangleTop_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <math.h>
#include <limits>
#include "Referentials.h"

#define PI 3.141592653589793238463
#define EPSILON 0.00000001

TriangleTop_t::TriangleTop_t(Material_t *material, TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix) {}

TriangleTop_t::~TriangleTop_t(){}

void TriangleTop_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {
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

void TriangleTop_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {

}

void TriangleTop_t::normal_face(Vec3f &normalvec) const{

}

Vec3f TriangleTop_t::mincoord() const {
    return Vec3f();
}

Vec3f TriangleTop_t::maxcoord() const {
    return Vec3f();
}