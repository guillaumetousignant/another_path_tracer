#include "DirectionalLight_t.h"
#include "TransformMatrix_t.h"

DirectionalLight_t::DirectionalLight_t(const Vec3f &intensity, TransformMatrix_t* transformation) : intensity_(intensity), transformation_(transformation) {
    TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    radius_ = Vec3f(transformation_->matrix_[0], transformation_->matrix_[5], transformation_->matrix_[10]).magnitude();
}

DirectionalLight_t::~DirectionalLight_t(){}

void DirectionalLight_t::update(){
    TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    radius_ = Vec3f(transformation_->matrix_[0], transformation_->matrix_[5], transformation_->matrix_[10]).magnitude();
}