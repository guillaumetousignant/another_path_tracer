#include "entities/DirectionalLight_t.hpp"

using APTracer::Entities::DirectionalLight_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Vec3f;

DirectionalLight_t::DirectionalLight_t(const Vec3f& intensity, TransformMatrix_t* transformation) : intensity_(intensity), transformation_(transformation) {
    direction_ = transformation_->multDir(Vec3f(0.0, 1.0, 0.0));
    radius_    = transformation_->getScale();
}

auto DirectionalLight_t::update() -> void {
    direction_ = transformation_->multDir(Vec3f(0.0, 1.0, 0.0));
    radius_    = transformation_->getScale();
}
