#include "IsoCam_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "RandomGenerator_t.h"
#include <cmath>

#define PI 3.141592653589793238463

IsoCam_t::IsoCam_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), image_(image), unif_(0.0, 1.0) {}

IsoCam_t::~IsoCam_t() {}

void IsoCam_t::update() {
    origin_ = transformation_->multVec(Vec3f());
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    up_ = up_buffer_;
}

void IsoCam_t::raytrace(const Scene_t* scene) {
    const Vec3f horizontal = direction_.cross(up_).normalize();
    const Vec3f vertical = horizontal.cross(direction_).normalize();
    const double tot_subpix = subpix_[0]*subpix_[1];
    const double pixel_span_y = fov_[0]/image_->size_y_;
    const double pixel_span_x = fov_[1]/image_->size_x_;
    const double subpix_span_y = pixel_span_y/subpix_[0];
    const double subpix_span_x = pixel_span_x/subpix_[1];
    #ifdef _WIN32
        int index; // Openmp on windows can't use unsigned index.
    #else
        unsigned int index;
    #endif

    image_->update();

    #pragma omp parallel for schedule(guided)
    for (index = 0; index < image_->size_y_ * image_->size_x_; ++index){
        const unsigned int i = index%image_->size_x_;
        const unsigned int j = index/image_->size_x_;
        Vec3f col = Vec3f(); // Or declare above?
        const Vec3f ray_origin = origin_ - vertical * pixel_span_y * ((double)j - (double)image_->size_y_/2.0 + 0.5) + horizontal * pixel_span_x * ((double)i - (double)image_->size_x_/2.0 + 0.5);

        for (unsigned int subindex = 0; subindex < subpix_[0] * subpix_[1]; ++subindex){
            const unsigned int l = subindex%subpix_[1]; // x
            const unsigned int k = subindex/subpix_[1]; // y
            const double jitter_y = unif_(my_rand::rng);
            const double jitter_x = unif_(my_rand::rng);

            const Vec3f subpix_ray_origin = ray_origin - vertical * subpix_span_y * ((double)k - (double)subpix_[0]/2.0 + jitter_y) + horizontal * subpix_span_x * ((double)l - (double)subpix_[1]/2.0 + jitter_x);;

            Ray_t ray = Ray_t(subpix_ray_origin, direction_, Vec3f(), Vec3f(1.0), medium_list_);
            ray.raycast(scene, max_bounces_, skybox_);
            col += ray.colour_;
        }
        col = col/tot_subpix;
        image_->update(col, i, j);
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