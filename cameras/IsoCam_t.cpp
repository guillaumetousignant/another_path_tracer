#include "IsoCam_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "Referentials.h"
#include "RandomGenerator_t.h"
#include <cmath>

#define PI 3.141592653589793238463

IsoCam_t::IsoCam_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), image_(image), unif_(0.0, 1.0) {}

IsoCam_t::~IsoCam_t() {}

void IsoCam_t::update() {
    origin_ = transformation_->multVec(Vec3f(0.0, 0.0, 0.0));
    TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    up_ = up_buffer_;
}

void IsoCam_t::raytrace(const Scene_t* scene) {
    double tot_subpix;
    double pixel_span_x, pixel_span_y;
    double subpix_span_x, subpix_span_y;
    Vec3f horizontal, vertical;
    
    horizontal = direction_.cross(up_).normalize();
    vertical = horizontal.cross(direction_).normalize();
    tot_subpix = subpix_[0]*subpix_[1];
    pixel_span_y = fov_[0]/image_->size_y_;
    pixel_span_x = fov_[1]/image_->size_x_;
    subpix_span_y = pixel_span_y/subpix_[0];
    subpix_span_x = pixel_span_x/subpix_[1];

    image_->update();

    #pragma omp parallel for collapse(2) schedule(guided)
    for (unsigned int j = 0; j < image_->size_y_; j++){
        for (unsigned int i = 0; i < image_->size_x_; i++){
            Vec3f col = Vec3f(); // Or declare above?
            Vec3f ray_origin = origin_ - vertical * pixel_span_y * ((double)j - (double)image_->size_y_/2.0 + 0.5) + horizontal * pixel_span_x * ((double)i - (double)image_->size_x_/2.0 + 0.5);

            for (unsigned int k = 0; k < subpix_[0]; k++){
                for (unsigned int l = 0; l < subpix_[1]; l++){
                    
                    double jitter_y = unif_(my_rand::rng);
                    double jitter_x = unif_(my_rand::rng);

                    Vec3f subpix_ray_origin = ray_origin - vertical * subpix_span_y * ((double)k - (double)subpix_[0]/2.0 + jitter_y) + horizontal * subpix_span_x * ((double)l - (double)subpix_[1]/2.0 + jitter_x);;

                    Ray_t ray = Ray_t(subpix_ray_origin, direction_, Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list_);
                    ray.raycast(scene, max_bounces_, skybox_);
                    col += ray.colour_;
                }
            }
            col = col/tot_subpix;
            image_->update(col, i, j);
        }
    }
}

void IsoCam_t::write(std::string file_name /*= ""*/) {
    if (file_name.empty()){
        file_name = filename_;
    }
    image_->write(file_name);
}

void IsoCam_t::show() const {
    // What to do here?
}

void IsoCam_t::reset(){
    image_->reset();
}