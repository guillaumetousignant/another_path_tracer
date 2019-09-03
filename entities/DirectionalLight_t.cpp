#include "DirectionalLight_t.h"
#include "TransformMatrix_t.h"

DirectionalLight_t::DirectionalLight_t(const Vec3f &intensity, TransformMatrix_t* transformation) : intensity_(intensity), transformation_(transformation) {
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    radius_ = transformation_->getScale(); 
}

DirectionalLight_t::~DirectionalLight_t(){}

void DirectionalLight_t::update(){
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    radius_ = transformation_->getScale();
}