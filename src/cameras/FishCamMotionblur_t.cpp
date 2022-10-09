#include "cameras/FishCamMotionblur_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Skybox_t.h"
#include "entities/Scene_t.h"
#include "entities/Medium_t.h"
#include "entities/ImgBuffer_t.h"
#include "entities/RandomGenerator_t.h"
#include <array>
#include <cmath>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;
using APTracer::Cameras::FishCamMotionblur_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;

FishCamMotionblur_t::FishCamMotionblur_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, std::array<double, 2> fov, std::array<unsigned int, 2> subpix, ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, std::array<double, 2> time, double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, std::move(medium_list), skybox, max_bounces, gammaind), 
    image_(image), unif_(0.0, 1.0), direction_last_(direction_), origin_last_(origin_), time_{time}, up_last_(up_), fov_last_{fov} {}

auto FishCamMotionblur_t::update() -> void {
    origin_last_ = origin_;
    direction_last_ = direction_;
    up_last_ = up_;
    fov_last_ = fov_;

    origin_ = transformation_->multVec(Vec3f());
    direction_ = transformation_->multDir(Vec3f(0.0, 1.0, 0.0));
    up_ = up_buffer_;
    fov_ = fov_buffer_;
}

auto FishCamMotionblur_t::raytrace(const Scene_t* scene) -> void {
    const double tot_subpix = subpix_[0]*subpix_[1];
    const double pixel_span_y_last = fov_last_[0]/image_->size_y_;
    const double pixel_span_x_last = fov_last_[1]/image_->size_x_;
    const double pixel_span_y = fov_[0]/image_->size_y_;
    const double pixel_span_x = fov_[1]/image_->size_x_;
    const double subpix_span_y_last = pixel_span_y_last/subpix_[0];
    const double subpix_span_x_last = pixel_span_x_last/subpix_[1];
    const double subpix_span_y = pixel_span_y/subpix_[0];
    const double subpix_span_x = pixel_span_x/subpix_[1];

    const Vec3f horizontal_last = direction_last_.cross(up_last_).normalize_inplace();
    const Vec3f vertical_last = horizontal_last.cross(direction_last_).normalize_inplace();
    const Vec3f horizontal = direction_.cross(up_).normalize_inplace();
    const Vec3f vertical = horizontal.cross(direction_).normalize_inplace();

    image_->update();

    #ifdef APTRACER_USE_OPENMP
    #pragma omp parallel for schedule(guided)
    #endif
    #ifdef _WIN32
    for (int index = 0; index < static_cast<int>(image_->size_y_ * image_->size_x_); ++index) { // Openmp on windows can't use unsigned index.
    #else
    for (size_t index = 0; index < image_->size_y_ * image_->size_x_; ++index) {
    #endif
        const size_t i = index%image_->size_x_;
        const size_t j = index/image_->size_x_;
        Vec3f col = Vec3f(); // Or declare above?
        
        for (unsigned int subindex = 0; subindex < subpix_[0] * subpix_[1]; ++subindex) {
            const unsigned int l = subindex%subpix_[1]; // x
            const unsigned int k = subindex/subpix_[1]; // y
            const double rand_time = unif_(APTracer::Entities::rng()) * (time_[1] - time_[0]) + time_[0];
            const double jitter_y = unif_(APTracer::Entities::rng());
            const double jitter_x = unif_(APTracer::Entities::rng());

            const Vec3f direction_int = direction_ * rand_time + direction_last_ * (1.0 - rand_time); // maybe should normalise this
            const Vec3f horizontal_int = horizontal * rand_time + horizontal_last * (1.0 - rand_time); // maybe should normalise this
            const Vec3f vertical_int = vertical * rand_time + vertical_last * (1.0 - rand_time); // maybe should normalise this
            const Vec3f origin_int = origin_ * rand_time + origin_last_ * (1.0 - rand_time);
            const double pixel_span_y_int = pixel_span_y * rand_time + pixel_span_y_last * (1.0 - rand_time);
            const double pixel_span_x_int = pixel_span_x * rand_time + pixel_span_x_last * (1.0 - rand_time);
            const double subpix_span_y_int = subpix_span_y * rand_time + subpix_span_y_last * (1.0 - rand_time);
            const double subpix_span_x_int = subpix_span_x * rand_time + subpix_span_x_last * (1.0 - rand_time);

            const std::array<double, 2> subpix_position {(static_cast<double>(i) - static_cast<double>(image_->size_x_)/2.0 + 0.5)*pixel_span_x_int
                                                            + (static_cast<double>(l) - static_cast<double>(subpix_[1])/2.0 + jitter_x)*subpix_span_x_int,
                                                        -(static_cast<double>(j) - static_cast<double>(image_->size_y_)/2.0 + 0.5)*pixel_span_y_int
                                                            + (static_cast<double>(k) - static_cast<double>(subpix_[0])/2.0 + jitter_y)*subpix_span_y_int};

            const double theta = std::sqrt(std::pow(subpix_position[0], 2) + std::pow(subpix_position[1], 2));
            const Vec3f plane_vector = horizontal_int * subpix_position[0]/theta + vertical_int * subpix_position[1]/theta;
            const Vec3f subpix_vec = std::cos(theta) * direction_int + std::sin(theta) * plane_vector;

            Ray_t ray = Ray_t(origin_int, subpix_vec.normalize(), Vec3f(), Vec3f(1.0), medium_list_, rand_time);
            ray.raycast(scene, max_bounces_, skybox_);
            col += ray.colour_;
        }
        col = col/tot_subpix;
        image_->update(col, i, j);        
    }
}

auto FishCamMotionblur_t::zoom(double factor) -> void {
    fov_buffer_ = {fov_[0] * factor, fov_[1] * factor};
}

auto FishCamMotionblur_t::write(const std::string& file_name) -> void {
    image_->write(file_name, gammaind_);
}

auto FishCamMotionblur_t::write() -> void {
    image_->write(filename_, gammaind_);
}

auto FishCamMotionblur_t::show() const -> void {
    // What to do here?
}

auto FishCamMotionblur_t::reset() -> void {
    image_->reset();
}