#include "shapes/Sphere_t.hpp"
#include <cmath>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Shapes::Sphere_t::Sphere_t(APTracer::Entities::Material_t* material, APTracer::Entities::TransformMatrix_t* transform_matrix) : Shape_t(material, transform_matrix) {
    origin_                = transformation_->multVec(Vec3f());
    radius_                = transformation_->getScale();
    const Vec3f direction  = transformation_->multDir(Vec3f(0.0, 0.0, 1.0)).to_sph();
    const Vec3f direction2 = transformation_->multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_         = Vec3f(1.0, direction[1], direction2[2]);
}

auto APTracer::Shapes::Sphere_t::update() -> void {
    origin_                = transformation_->multVec(Vec3f());
    radius_                = transformation_->getScale();
    const Vec3f direction  = transformation_->multDir(Vec3f(0.0, 0.0, 1.0)).to_sph();
    const Vec3f direction2 = transformation_->multDir(Vec3f(1.0, 0.0, 0.0)).to_sph();
    direction_sph_         = Vec3f(1.0, direction[1], direction2[2]);
}

auto APTracer::Shapes::Sphere_t::intersection(const APTracer::Entities::Ray_t& ray, double& t, std::array<double, 2>& uv) const -> bool {
    const Vec3f to_center     = origin_ - ray.origin_;
    const double b            = to_center.dot(ray.direction_);
    const double c            = to_center.dot(to_center) - pow(radius_, 2);
    const double discriminant = pow(b, 2) - c;

    if (discriminant < 0.0) {
        return false;
    }
    t = b - sqrt(discriminant);

    if (t < 0.0) {
        t = b + sqrt(discriminant);
        if (t < 0.0) {
            return false;
        }
    }

    const Vec3f sph = (ray.direction_ * t - to_center).to_sph();
    uv              = {sph[2] / (2.0 * pi) + 0.5, 1.0 - sph[1] / pi};
    return true;
}

auto APTracer::Shapes::Sphere_t::normaluv(double time, std::array<double, 2> uv, std::array<double, 2>& tuv) const -> Vec3f {
    Vec3f sph             = Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi);
    const Vec3f normalvec = sph.get_xyz();

    sph -= direction_sph_;

    // CHECK change
    if (sph[1] < 0.0) {
        sph[1] = -sph[1];
        sph[2] += pi;
    }
    else if (sph[1] > pi) {
        sph[1] = 2.0 * pi - sph[1];
        sph[2] += pi;
    }

    // CHECK change
    if (sph[2] < -pi) {
        sph[2] += 2.0 * pi;
    }
    else if (sph[2] > pi) {
        sph[2] -= 2.0 * pi;
    }

    tuv = {sph[2] / (2.0 * pi) + 0.5, 1.0 - sph[1] / pi};
    return normalvec;
}

auto APTracer::Shapes::Sphere_t::normal(double time, std::array<double, 2> uv) const -> Vec3f {
    return Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi).to_xyz();
}

auto APTracer::Shapes::Sphere_t::normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2>& tuv, Vec3f& tangentvec) const -> Vec3f {
    Vec3f sph             = Vec3f(1.0, (1.0 - uv[1]) * pi, (uv[0] - 0.5) * 2.0 * pi);
    const Vec3f normalvec = sph.get_xyz();

    sph -= direction_sph_;

    // CHECK change
    if (sph[1] < 0.0) {
        sph[1] = -sph[1];
        sph[2] += pi;
    }
    else if (sph[1] > pi) {
        sph[1] = 2.0 * pi - sph[1];
        sph[2] += pi;
    }

    // CHECK change
    if (sph[2] < -pi) {
        sph[2] += 2.0 * pi;
    }
    else if (sph[2] > pi) {
        sph[2] -= 2.0 * pi;
    }

    tuv = {sph[2] / (2.0 * pi) + 0.5, 1.0 - sph[1] / pi};

    tangentvec = direction_sph_.get_xyz().cross(normalvec).normalize_inplace();
    return normalvec;
}

auto APTracer::Shapes::Sphere_t::mincoord() const -> Vec3f {
    return origin_ - radius_;
}

auto APTracer::Shapes::Sphere_t::maxcoord() const -> Vec3f {
    return origin_ + radius_;
}