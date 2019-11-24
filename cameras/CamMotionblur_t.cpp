#include "CamMotionblur_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "RandomGenerator_t.h"

#define PI 3.141592653589793238463

CamMotionblur_t::CamMotionblur_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double (&time)[2], double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), 
    image_(image), unif_(0.0, 1.0), direction_last_(direction_), origin_last_(origin_), time_{time[0], time[1]}, up_last_(up_) {}

CamMotionblur_t::~CamMotionblur_t() {}

void CamMotionblur_t::update() {
    origin_last_ = origin_;
    direction_last_ = direction_;
    up_last_ = up_;

    origin_ = transformation_->multVec(Vec3f());
    const TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    up_ = up_buffer_;
}

void CamMotionblur_t::raytrace(const Scene_t* scene) {
    const double tot_subpix = subpix_[0]*subpix_[1];
    const double pixel_span_y = fov_[0]/image_->size_y_;
    const double pixel_span_x = fov_[1]/image_->size_x_;
    const double subpix_span_y = pixel_span_y/subpix_[0];
    const double subpix_span_x = pixel_span_x/subpix_[1];

    const Vec3f horizontal_last = direction_last_.cross(up_last_).normalize();
    const Vec3f vertical_last = horizontal_last.cross(direction_last_).normalize();
    const Vec3f horizontal = direction_.cross(up_).normalize();
    const Vec3f vertical = horizontal.cross(direction_).normalize();

    #ifdef _WIN32
        int index; // Openmp on windows can't use unsigned index.
    #else
        unsigned int index;
    #endif

    image_->update();

    #pragma omp parallel for schedule(guided)
    for (index = 0; index < image_->size_y_ * image_->size_x_; ++index){
        const unsigned int i = index%image_->size_y_;
        const unsigned int j = index/image_->size_y_;
        Vec3f col = Vec3f(); // Or declare above?
        const Vec3f pix_vec = Vec3f(1.0, PI/2.0 + ((double)j - (double)image_->size_y_/2.0 + 0.5)*pixel_span_y, ((double)i - (double)image_->size_x_/2.0 + 0.5)*pixel_span_x);
        
        for (unsigned int subindex = 0; subindex < subpix_[0] * subpix_[1]; ++subindex){
            const unsigned int l = subindex%subpix_[0]; // x
            const unsigned int k = subindex/subpix_[0]; // y
            const double rand_time = unif_(my_rand::rng) * (time_[1] - time_[0]) + time_[0];
            const double jitter_y = unif_(my_rand::rng);
            const double jitter_x = unif_(my_rand::rng);

            const Vec3f direction_int = direction_ * rand_time + direction_last_ * (1.0 - rand_time); // maybe should normalise this
            const Vec3f horizontal_int = horizontal * rand_time + horizontal_last * (1.0 - rand_time); // maybe should normalise this
            const Vec3f vertical_int = vertical * rand_time + vertical_last * (1.0 - rand_time); // maybe should normalise this
            const Vec3f origin_int = origin_ * rand_time + origin_last_ * (1.0 - rand_time);

            Vec3f subpix_vec = (pix_vec + Vec3f(0.0, ((double)k - (double)subpix_[0]/2.0 + jitter_y)*subpix_span_y, ((double)l - (double)subpix_[1]/2.0 + jitter_x)*subpix_span_x)).to_xyz_offset(direction_int, horizontal_int, vertical_int).normalize();

            Ray_t ray = Ray_t(origin_int, subpix_vec, Vec3f(), Vec3f(1.0), medium_list_, rand_time);
            ray.raycast(scene, max_bounces_, skybox_);
            col += ray.colour_;
        }
        col = col/tot_subpix;
        image_->update(col, i, j);        
    }
}

void CamMotionblur_t::write(std::string file_name /*= ""*/) {
    if (file_name.empty()){
        file_name = filename_;
    }
    image_->write(file_name, gammaind_);
}

void CamMotionblur_t::show() const {
    // What to do here?
}

void CamMotionblur_t::reset(){
    image_->reset();
}