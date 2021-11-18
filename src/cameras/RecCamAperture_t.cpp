#include "cameras/RecCamAperture_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Skybox_t.h"
#include "entities/Scene_t.h"
#include "entities/Medium_t.h"
#include "entities/ImgBuffer_t.h"
#include "entities/RandomGenerator_t.h"
#include <cmath>
#include <limits>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;
using APTracer::Cameras::RecCamAperture_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;

RecCamAperture_t::RecCamAperture_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, std::array<double, 2> fov, std::array<unsigned int, 2> subpix, ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focus_distance, double aperture, double gammaind) 
    : Camera_t(transformation, filename, up, fov, subpix, std::move(medium_list), skybox, max_bounces, gammaind),  
    image_(image), unif_(0.0, 1.0), focus_distance_(focus_distance), aperture_(aperture), focus_distance_buffer_(focus_distance) {}

auto RecCamAperture_t::update() -> void {
    origin_ = transformation_->multVec(Vec3f());
    direction_ = transformation_->multDir(Vec3f(0.0, 1.0, 0.0));
    focus_distance_ = focus_distance_buffer_;
    up_ = up_buffer_;
    fov_ = fov_buffer_;
}

auto RecCamAperture_t::raytrace(const Scene_t* scene) -> void {
    const Vec3f horizontal = direction_.cross(up_).normalize_inplace();
    const Vec3f vertical = horizontal.cross(direction_).normalize_inplace();
    const Vec3f focus_point = origin_ + direction_ * focus_distance_;
    const double tot_subpix = subpix_[0]*subpix_[1];
    const Vec3f pixel_span_y = vertical * focus_distance_ * std::tan(fov_[0]/2.0) * 2.0/static_cast<double>(image_->size_y_);
    const Vec3f pixel_span_x = horizontal * focus_distance_ * std::tan(fov_[1]/2.0) * 2.0/static_cast<double>(image_->size_x_);
    const Vec3f subpix_span_y = pixel_span_y/subpix_[0];
    const Vec3f subpix_span_x = pixel_span_x/subpix_[1];

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
        const Vec3f pix_vec = focus_point - pixel_span_y * (static_cast<double>(j) - static_cast<double>(image_->size_y_)/2.0 + 0.5) + pixel_span_x * (static_cast<double>(i) - static_cast<double>(image_->size_x_)/2.0 + 0.5);
        
        for (unsigned int subindex = 0; subindex < subpix_[0] * subpix_[1]; ++subindex) {
            const unsigned int l = subindex%subpix_[1]; // x
            const unsigned int k = subindex/subpix_[1]; // y
            const double rand_theta = unif_(APTracer::Entities::rng()) * 2.0 * pi;
            const double rand_r = std::sqrt(unif_(APTracer::Entities::rng())) * aperture_;
            const double jitter_y = unif_(APTracer::Entities::rng());
            const double jitter_x = unif_(APTracer::Entities::rng());

            const Vec3f origin2 = origin_ + vertical * std::cos(rand_theta) * rand_r + horizontal * std::sin(rand_theta) * rand_r;

            const Vec3f subpix_vec = (pix_vec - subpix_span_y * (static_cast<double>(k) - static_cast<double>(subpix_[0])/2.0 + jitter_y) + subpix_span_x * (static_cast<double>(l) - static_cast<double>(subpix_[1])/2.0 + jitter_x) - origin2).normalize_inplace();
            
            Ray_t ray = Ray_t(origin2, subpix_vec, Vec3f(), Vec3f(1.0), medium_list_);
            ray.raycast(scene, max_bounces_, skybox_);
            col += ray.colour_;
        }
        col = col/tot_subpix;
        image_->update(col, i, j);
    }
}

auto RecCamAperture_t::focus(double focus_distance) -> void {
    focus_distance_buffer_ = focus_distance;
}

auto RecCamAperture_t::autoFocus(const Scene_t* scene, std::array<double, 2> position) -> void {
    double t = std::numeric_limits<double>::max();
    std::array<double, 2> uv;

    const Vec3f horizontal = direction_.cross(up_).normalize_inplace();
    const Vec3f vertical = horizontal.cross(direction_).normalize_inplace();
    const double span_x = 2 * std::tan(fov_[1]/2.0);
    const double span_y = 2 * std::tan(fov_[0]/2.0);
    const Vec3f ray_vec = (direction_
                           - span_y * vertical * (position[1] - 0.5) 
                           + span_x * horizontal * (position[0] - 0.5)).normalize_inplace();

    const Ray_t focus_ray = Ray_t(origin_, ray_vec, Vec3f(), Vec3f(1.0), medium_list_);

    if (scene->intersect(focus_ray, t, uv) == nullptr) {
        t = 1000000;
    }
    else {
        t = direction_.dot(t * ray_vec);
    }
    focus(t);
}

auto RecCamAperture_t::zoom(double factor) -> void {
    const double span_x = std::tan(fov_[1]/2.0);
    const double span_y = std::tan(fov_[0]/2.0);
    
    fov_buffer_ = {2 * std::atan(span_y * factor), 2 * std::atan(span_x * factor)};
}

auto RecCamAperture_t::write(const std::string& file_name) -> void {
    image_->write(file_name);
}

auto RecCamAperture_t::write() -> void {
    image_->write(filename_);
}

auto RecCamAperture_t::show() const -> void {
    // What to do here?
}

auto RecCamAperture_t::reset() -> void {
    image_->reset();
}
