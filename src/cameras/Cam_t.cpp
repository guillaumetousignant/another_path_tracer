#include "cameras/Cam_t.hpp"
#include "entities/RandomGenerator_t.hpp"

constexpr double pi = 3.141592653589793238463;

using APTracer::Cameras::Cam_t;
using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Vec3f;

Cam_t::Cam_t(TransformMatrix_t* transformation,
             const std::string& filename,
             Vec3f up,
             std::array<double, 2> fov,
             std::array<unsigned int, 2> subpix,
             ImgBuffer_t* image,
             std::list<Medium_t*> medium_list,
             Skybox_t* skybox,
             unsigned int max_bounces,
             double gammaind) :
        Camera_t(transformation, filename, up, fov, subpix, std::move(medium_list), skybox, max_bounces, gammaind), image_(image), unif_(0.0, 1.0) {}

auto Cam_t::update() -> void {
    origin_    = transformation_->multVec(Vec3f());
    direction_ = transformation_->multDir(Vec3f(0.0, 1.0, 0.0));
    up_        = up_buffer_;
    fov_       = fov_buffer_;
}

auto Cam_t::raytrace(const Scene_t* scene) -> void {
    const double tot_subpix    = subpix_[0] * subpix_[1];
    const double pixel_span_y  = fov_[0] / static_cast<double>(image_->size_y_);
    const double pixel_span_x  = fov_[1] / static_cast<double>(image_->size_x_);
    const double subpix_span_y = pixel_span_y / subpix_[0];
    const double subpix_span_x = pixel_span_x / subpix_[1];
    const Vec3f horizontal     = direction_.cross(up_).normalize_inplace();
    const Vec3f vertical       = horizontal.cross(direction_).normalize_inplace();

    image_->update();

#ifdef APTRACER_USE_OPENMP
    #pragma omp parallel for schedule(guided)
#endif
#ifdef _WIN32
    for (int index = 0; index < static_cast<int>(image_->size_y_ * image_->size_x_); ++index) { // Openmp on windows can't use unsigned index.
#else
    for (size_t index = 0; index < image_->size_y_ * image_->size_x_; ++index) {
#endif
        const size_t i      = index % image_->size_x_;
        const size_t j      = index / image_->size_x_;
        Vec3f col           = Vec3f(); // Or declare above?
        const Vec3f pix_vec = Vec3f(1.0,
                                    pi / 2.0 + (static_cast<double>(j) - static_cast<double>(image_->size_y_) / 2.0 + 0.5) * pixel_span_y,
                                    (static_cast<double>(i) - static_cast<double>(image_->size_x_) / 2.0 + 0.5) * pixel_span_x);

        for (unsigned int subindex = 0; subindex < subpix_[0] * subpix_[1]; ++subindex) {
            const unsigned int l  = subindex % subpix_[1]; // x
            const unsigned int k  = subindex / subpix_[1]; // y
            const double jitter_y = unif_(APTracer::Entities::rng());
            const double jitter_x = unif_(APTracer::Entities::rng());

            const Vec3f subpix_vec = (pix_vec
                                      + Vec3f(0.0,
                                              (static_cast<double>(k) - static_cast<double>(subpix_[0]) / 2.0 + jitter_y) * subpix_span_y,
                                              (static_cast<double>(l) - static_cast<double>(subpix_[1]) / 2.0 + jitter_x) * subpix_span_x))
                                         .to_xyz_offset(direction_, horizontal, vertical);

            Ray_t ray = Ray_t(origin_, subpix_vec, Vec3f(), Vec3f(1.0), medium_list_);
            scene->raycast(ray, max_bounces_, skybox_);
            col += ray.colour_;
        }
        col = col / tot_subpix;
        image_->update(col, i, j);
    }
}

auto Cam_t::zoom(double factor) -> void {
    fov_buffer_ = {fov_[0] * factor, fov_[1] * factor};
}

auto Cam_t::write(const std::string& file_name) -> void {
    image_->write(file_name, gammaind_);
}

auto Cam_t::write() -> void {
    image_->write(filename_, gammaind_);
}

auto Cam_t::show() const -> void {
    // What to do here?
}

auto Cam_t::reset() -> void {
    image_->reset();
}