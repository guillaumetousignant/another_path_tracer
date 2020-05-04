#include "cameras/RecCam_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Skybox_t.h"
#include "entities/Scene_t.h"
#include "entities/Medium_t.h"
#include "entities/ImgBuffer_t.h"
#include "entities/RandomGenerator_t.h"
#include <cmath>

#define PI 3.141592653589793238463

using APTracer::Entities::Vec3f;
using APTracer::Cameras::RecCam_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;

RecCam_t::RecCam_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), image_(image), unif_(0.0, 1.0) {}

RecCam_t::~RecCam_t() {}

void RecCam_t::update() {
    origin_ = transformation_->multVec(Vec3f());
    direction_ = transformation_->transformDir().multDir(Vec3f(0.0, 1.0, 0.0));
    up_ = up_buffer_;
}

void RecCam_t::raytrace(const Scene_t* scene) {
    const Vec3f horizontal = direction_.cross(up_).normalize_inplace();
    const Vec3f vertical = horizontal.cross(direction_).normalize_inplace();
    const Vec3f focus_point = origin_ + direction_;
    const double tot_subpix = subpix_[0]*subpix_[1];
    const Vec3f pixel_span_y = vertical * std::tan(fov_[0]/2.0) * 2.0/image_->size_y_;
    const Vec3f pixel_span_x = horizontal * std::tan(fov_[1]/2.0) * 2.0/image_->size_x_;
    const Vec3f subpix_span_y = pixel_span_y/subpix_[0];
    const Vec3f subpix_span_x = pixel_span_x/subpix_[1];
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
        const Vec3f pix_vec = focus_point - pixel_span_y * ((double)j - (double)image_->size_y_/2.0 + 0.5) + pixel_span_x * ((double)i - (double)image_->size_x_/2.0 + 0.5);

        for (unsigned int subindex = 0; subindex < subpix_[0] * subpix_[1]; ++subindex){
            const unsigned int l = subindex%subpix_[1]; // x
            const unsigned int k = subindex/subpix_[1]; // y
            const double jitter_y = unif_(APTracer::Entities::rng);
            const double jitter_x = unif_(APTracer::Entities::rng);

            const Vec3f subpix_vec = (pix_vec - subpix_span_y * ((double)k - (double)subpix_[0]/2.0 + jitter_y) + subpix_span_x * ((double)l - (double)subpix_[1]/2.0 + jitter_x) - origin_).normalize_inplace();
            
            Ray_t ray = Ray_t(origin_, subpix_vec, Vec3f(), Vec3f(1.0), medium_list_);
            ray.raycast(scene, max_bounces_, skybox_);
            col += ray.colour_;
        }
        col = col/tot_subpix;
        image_->update(col, i, j);
    }
}

void RecCam_t::write(std::string file_name /*= ""*/) {
    if (file_name.empty()){
        file_name = filename_;
    }
    image_->write(file_name);
}

void RecCam_t::show() const {
    // What to do here?
}

void RecCam_t::reset(){
    image_->reset();
}