#include "shapes/TriangleMotionblur_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Material_t.h"
#include <cmath>
#include <limits>
#include <algorithm>

constexpr double epsilon = 0.00000001;

using APTracer::Entities::Vec3f;

APTracer::Shapes::TriangleMotionblur_t::TriangleMotionblur_t(APTracer::Entities::Material_t *material, APTracer::Entities::TransformMatrix_t *transform_matrix, std::array<Vec3f, 3> points, const Vec3f* normals, const double* texcoord) 
    : Shape_t(material, transform_matrix), points_orig_{points} {

    if (normals == nullptr) {
        const Vec3f nor = (points_orig_[1] - points_orig_[0]).cross(points_orig_[2] - points_orig_[0]).normalize_inplace(); 
        normals_orig_ = {nor,
                         nor,
                         nor};
    }
    else {
        normals_orig_ = {normals[0],
                         normals[1],
                         normals[2]};
    }

    if (texcoord == nullptr) {
        texture_coordinates_ = {0, 1,
                                0, 0,
                                1, 0};
    }
    else {
        texture_coordinates_ = {texcoord[0], texcoord[1],
                                texcoord[2], texcoord[3],
                                texcoord[4], texcoord[5]};
    }

    points_ = {transformation_->multVec(points_orig_[0]),
               transformation_->multVec(points_orig_[1]),
               transformation_->multVec(points_orig_[2])};
    normals_ = {transformation_->multDir(normals_orig_[0]),
                transformation_->multDir(normals_orig_[1]),
                transformation_->multDir(normals_orig_[2])};
    points_last_ = points_;
    normals_last_ = normals_;

    v0v1_ = points_[1] - points_[0];
    v0v1_last_ = v0v1_;
    v0v2_ = points_[2] - points_[0];
    v0v2_last_ = v0v2_;

    const std::array<double, 2> tuv0v1 {texture_coordinates_[2] - texture_coordinates_[0], texture_coordinates_[3] - texture_coordinates_[1]};
    const std::array<double, 2> tuv0v2 {texture_coordinates_[4] - texture_coordinates_[0], texture_coordinates_[5] - texture_coordinates_[1]};    
    const double invdet = 1.0/(tuv0v1[0] * tuv0v2[1] - tuv0v1[1] * tuv0v2[0]);
    if (std::isfinite(invdet)) {
        tuv_to_world_ = {invdet * -tuv0v2[0], invdet * tuv0v1[0]};
    }
    else {
        tuv_to_world_ = {1.0, 0.0};
    }

    tangent_vec_ = v0v1_ * tuv_to_world_[0] + v0v2_ * tuv_to_world_[1];
    tangent_vec_last_ = tangent_vec_;
}

auto APTracer::Shapes::TriangleMotionblur_t::update() -> void {    
    points_last_ = points_;
    normals_last_ = normals_;
    points_ = {transformation_->multVec(points_orig_[0]),
               transformation_->multVec(points_orig_[1]),
               transformation_->multVec(points_orig_[2])};
    normals_ = {transformation_->multDir(normals_orig_[0]),
                transformation_->multDir(normals_orig_[1]),
                transformation_->multDir(normals_orig_[2])};

    v0v1_last_ = v0v1_;
    v0v2_last_ = v0v2_;
    v0v1_ = points_[1] - points_[0];
    v0v2_ = points_[2] - points_[0];

    tangent_vec_last_ = tangent_vec_;
    tangent_vec_ = v0v1_ * tuv_to_world_[0] + v0v2_ * tuv_to_world_[1];
}

auto APTracer::Shapes::TriangleMotionblur_t::intersection(const APTracer::Entities::Ray_t &ray, double &t, std::array<double, 2> &uv) const -> bool {
    const Vec3f v0v1_int = v0v1_ * ray.time_ + v0v1_last_ * (1.0 - ray.time_);
    const Vec3f v0v2_int = v0v2_ * ray.time_ + v0v2_last_ * (1.0 - ray.time_);

    const std::array<Vec3f, 3> points_int {points_[0] * ray.time_ + points_last_[0] * (1.0 - ray.time_),
                                           points_[1] * ray.time_ + points_last_[1] * (1.0 - ray.time_),
                                           points_[2] * ray.time_ + points_last_[2] * (1.0 - ray.time_)};

    const Vec3f pvec = ray.direction_.cross(v0v2_int);
    const double det = v0v1_int.dot(pvec);

    if (std::abs(det) < epsilon) {
        t = std::numeric_limits<double>::infinity();
        uv = {NAN, NAN};
        return false;
    }

    const double invdet = 1.0/det;
    const Vec3f tvec = ray.origin_ - points_int[0];
    const double u = tvec.dot(pvec) * invdet;
    uv[0] = u;

    if ((u < 0.0) || (u > 1.0)) {
        t = std::numeric_limits<double>::infinity();
        uv[1] = NAN;
        return false;
    }

    const Vec3f qvec = tvec.cross(v0v1_int);
    const double v = ray.direction_.dot(qvec) * invdet;
    uv[1] = v;

    if ((v < 0.0) || ((u+v) > 1.0)) {
        t = std::numeric_limits<double>::infinity();
        return false;
    }

    t = v0v2_int.dot(qvec) * invdet;

    if (t < 0.0) {
        t = std::numeric_limits<double>::infinity();
        return false;
    }

    return true;
}

auto APTracer::Shapes::TriangleMotionblur_t::normaluv(double time, std::array<double, 2> uv, std::array<double, 2> &tuv) const -> Vec3f {
    const std::array<Vec3f, 3> normals_int {normals_[0] * time + normals_last_[0] * (1.0 - time),
                                            normals_[1] * time + normals_last_[1] * (1.0 - time),
                                            normals_[2] * time + normals_last_[2] * (1.0 - time)};

    const Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    // Matrix multiplication, optimise.
    tuv = {distance[0] * texture_coordinates_[0] + distance[1] * texture_coordinates_[2] + distance[2] * texture_coordinates_[4],
           distance[0] * texture_coordinates_[1] + distance[1] * texture_coordinates_[3] + distance[2] * texture_coordinates_[5]};
    return {distance[0] * normals_int[0][0] + distance[1] * normals_int[1][0] + distance[2] * normals_int[2][0], 
        distance[0] * normals_int[0][1] + distance[1] * normals_int[1][1] + distance[2] * normals_int[2][1],
        distance[0] * normals_int[0][2] + distance[1] * normals_int[1][2] + distance[2] * normals_int[2][2]};
}

auto APTracer::Shapes::TriangleMotionblur_t::normal(double time, std::array<double, 2> uv) const -> Vec3f {
    const std::array<Vec3f, 3> normals_int {normals_[0] * time + normals_last_[0] * (1.0 - time),
                                            normals_[1] * time + normals_last_[1] * (1.0 - time),
                                            normals_[2] * time + normals_last_[2] * (1.0 - time)};

    const Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    return {distance[0] * normals_int[0][0] + distance[1] * normals_int[1][0] + distance[2] * normals_int[2][0], 
        distance[0] * normals_int[0][1] + distance[1] * normals_int[1][1] + distance[2] * normals_int[2][1],
        distance[0] * normals_int[0][2] + distance[1] * normals_int[1][2] + distance[2] * normals_int[2][2]};
    // Matrix multiplication, optimise.
}

auto APTracer::Shapes::TriangleMotionblur_t::normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2> &tuv, Vec3f &tangentvec) const -> Vec3f {
    const std::array<Vec3f, 3> normals_int {normals_[0] * time + normals_last_[0] * (1.0 - time),
                                            normals_[1] * time + normals_last_[1] * (1.0 - time),
                                            normals_[2] * time + normals_last_[2] * (1.0 - time)};

    const Vec3f distance = Vec3f(1.0 - uv[0] - uv[1], uv[0], uv[1]);
    // Matrix multiplication, optimise.
    tuv = {distance[0] * texture_coordinates_[0] + distance[1] * texture_coordinates_[2] + distance[2] * texture_coordinates_[4],
           distance[0] * texture_coordinates_[1] + distance[1] * texture_coordinates_[3] + distance[2] * texture_coordinates_[5]};

    const Vec3f tangent_vec_int = tangent_vec_ * time + tangent_vec_last_ * (1.0 - time);

    const Vec3f normalvec = Vec3f(distance[0] * normals_int[0][0] + distance[1] * normals_int[1][0] + distance[2] * normals_int[2][0], 
        distance[0] * normals_int[0][1] + distance[1] * normals_int[1][1] + distance[2] * normals_int[2][1],
        distance[0] * normals_int[0][2] + distance[1] * normals_int[1][2] + distance[2] * normals_int[2][2]);
    tangentvec = tangent_vec_int.cross(normalvec).normalize_inplace();

    return normalvec;
} 

auto APTracer::Shapes::TriangleMotionblur_t::normal_face(double time) const -> Vec3f {
    const Vec3f v0v1_int = v0v1_ * time + v0v1_last_ * (1.0 - time);
    const Vec3f v0v2_int = v0v2_ * time + v0v2_last_ * (1.0 - time);

    return v0v1_int.cross(v0v2_int).normalize_inplace();
}

auto APTracer::Shapes::TriangleMotionblur_t::mincoord() const -> Vec3f {
    return points_[0].getMin(points_[1]).min(points_[2]).min(points_last_[0]).min(points_last_[1]).min(points_last_[2]);
}

auto APTracer::Shapes::TriangleMotionblur_t::maxcoord() const -> Vec3f {
    return points_[0].getMax(points_[1]).max(points_[2]).max(points_last_[0]).max(points_last_[1]).max(points_last_[2]);
}