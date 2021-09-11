#include "cameras/Cam3DMotionblurAperture_t.h"
#include "cameras/CamMotionblurAperture_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Skybox_t.h"
#include "entities/Scene_t.h"
#include "entities/Medium_t.h"
#include "entities/ImgBuffer_t.h"
#include "entities/RandomGenerator_t.h"

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;
using APTracer::Cameras::Cam3DMotionblurAperture_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;

Cam3DMotionblurAperture_t::Cam3DMotionblurAperture_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, std::array<double, 2> fov, std::array<unsigned int, 2> subpix, ImgBuffer_t* image, ImgBuffer_t* image_L, ImgBuffer_t* image_R, double eye_dist, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focus_distance, double aperture, std::array<double, 2> time, double gammaind) :
    Camera_t(transformation, filename, up, fov, subpix, std::move(medium_list), skybox, max_bounces, gammaind), 
    image_(image), unif_(0.0, 1.0), eye_dist_(eye_dist/2.0), focus_distance_(focus_distance), focus_distance_buffer_(focus_distance), aperture_(aperture),
    direction_last_(direction_), origin_last_(origin_), focus_distance_last_(focus_distance_), time_{time}, up_last_(up_), fov_last_{fov} {

    std::string filename_L;
    std::string filename_R;
    
    const size_t point = filename.find_last_of('.');

    if (point != std::string::npos) {
        filename_L = filename.substr(0, point) + "_L" + filename.substr(point);
        filename_R = filename.substr(0, point) + "_R" + filename.substr(point);
    }
    else {
        filename_L = filename + "_L.png";
        filename_R = filename + "_R.png";
    }

    camera_L_ = std::unique_ptr<CamMotionblurAperture_t>(new CamMotionblurAperture_t(transformation, filename_L, up_, fov_, subpix_, image_L, medium_list_, skybox_, max_bounces_, focus_distance_, aperture_, time_, gammaind_));
    camera_R_ = std::unique_ptr<CamMotionblurAperture_t>(new CamMotionblurAperture_t(transformation, filename_R, up_, fov_, subpix_, image_R, medium_list_, skybox_, max_bounces_, focus_distance_, aperture_, time_, gammaind_));

    const Vec3f horizontal = direction_.cross(up).normalize_inplace();

    camera_L_->focus_distance_ = std::sqrt(focus_distance_*focus_distance_ + eye_dist_*eye_dist_);
    camera_R_->focus_distance_ = camera_L_->focus_distance_;
    camera_L_->focus_distance_ = camera_L_->focus_distance_;
    camera_R_->focus_distance_ = camera_R_->focus_distance_;

    camera_L_->origin_ = horizontal * -eye_dist_ + origin_;
    camera_R_->origin_ = horizontal * eye_dist_ + origin_;
    camera_L_->origin_last_ = camera_L_->origin_;
    camera_R_->origin_last_ = camera_R_->origin_;
    
    camera_L_->direction_ = (direction_ * focus_distance_ + horizontal * eye_dist_).normalize_inplace();
    camera_R_->direction_ = (direction_ * focus_distance_ - horizontal * eye_dist_).normalize_inplace();
    camera_L_->direction_last_ = camera_L_->direction_;
    camera_R_->direction_last_ = camera_R_->direction_;
}

auto Cam3DMotionblurAperture_t::update() -> void {
    origin_last_ = origin_;
    direction_last_ = direction_;
    focus_distance_last_ = focus_distance_;
    up_last_ = up_;
    fov_last_ = fov_;

    origin_ = transformation_->multVec(Vec3f());
    direction_ = transformation_->multDir(Vec3f(0.0, 1.0, 0.0));
    focus_distance_ = focus_distance_buffer_;
    up_ = up_buffer_;
    fov_ = fov_buffer_;

    camera_L_->focus_distance_last_ = camera_L_->focus_distance_;
    camera_R_->focus_distance_last_ = camera_R_->focus_distance_;
    camera_L_->origin_last_ = camera_L_->origin_;
    camera_R_->origin_last_ = camera_R_->origin_;
    camera_L_->direction_last_ = camera_L_->direction_;
    camera_R_->direction_last_ = camera_R_->direction_;
    camera_L_->up_last_ = camera_L_->up_;
    camera_R_->up_last_ = camera_R_->up_;
    camera_L_->fov_last_ = camera_L_->fov_;
    camera_R_->fov_last_ = camera_R_->fov_;

    camera_L_->focus_distance_ = std::sqrt(std::pow(focus_distance_, 2) + std::pow(eye_dist_, 2));
    camera_R_->focus_distance_ = camera_L_->focus_distance_;
    camera_L_->up_ = up_;
    camera_R_->up_ = up_;
    camera_L_->fov_ = fov_;
    camera_R_->fov_ = fov_;
    const Vec3f horizontal = direction_.cross(up_);
    camera_L_->origin_ = horizontal * -eye_dist_ + origin_;
    camera_R_->origin_ = horizontal * eye_dist_ + origin_;
    camera_L_->direction_ = (direction_ * focus_distance_ + horizontal * eye_dist_).normalize_inplace();
    camera_R_->direction_ = (direction_ * focus_distance_ - horizontal * eye_dist_).normalize_inplace();
}

auto Cam3DMotionblurAperture_t::raytrace(const Scene_t* scene) -> void {
    camera_L_->raytrace(scene);
    camera_R_->raytrace(scene);

    image_->update();
    for (size_t j = 0; j < image_->size_y_; j++) {
        for (size_t i = 0; i < image_->size_x_; i++) {
            image_->set(Vec3f(camera_L_->image_->img_[j*camera_L_->image_->size_x_ + i][0], camera_R_->image_->img_[j*camera_L_->image_->size_x_ + i][1], camera_R_->image_->img_[j*camera_L_->image_->size_x_ + i][2]), i, j);
        }
    }
}

auto Cam3DMotionblurAperture_t::zoom(double factor) -> void {
    fov_buffer_ = {fov_[0] * factor, fov_[1] * factor};
}

auto Cam3DMotionblurAperture_t::write(const std::string& file_name) -> void {
    std::string filename_L;
    std::string filename_R;

    const size_t point = file_name.find_last_of('.');
    if (point != std::string::npos) {
        filename_L = file_name.substr(0, point) + "_L" + file_name.substr(point);
        filename_R = file_name.substr(0, point) + "_R" + file_name.substr(point);
    }
    else {
        filename_L = file_name + "_L.png";
        filename_R = file_name + "_R.png";
    }

    camera_L_->write(filename_L);
    camera_R_->write(filename_R);
    image_->write(file_name);
}

auto Cam3DMotionblurAperture_t::write() -> void {
    camera_L_->write();
    camera_R_->write();
    image_->write(filename_);
}

auto Cam3DMotionblurAperture_t::show() const -> void {
    // What to do here?
}

auto Cam3DMotionblurAperture_t::reset() -> void {
    camera_L_->reset();
    camera_R_->reset();
    image_->reset();
}

auto Cam3DMotionblurAperture_t::focus(double focus_distance) -> void {
    focus_distance_buffer_ = focus_distance;
}

auto Cam3DMotionblurAperture_t::autoFocus(const Scene_t* scene, std::array<double, 2> position) -> void {
    double t = std::numeric_limits<double>::infinity();
    std::array<double, 2> uv;

    const Vec3f horizontal = direction_.cross(up_).normalize_inplace();
    const Vec3f vertical = horizontal.cross(direction_).normalize_inplace();

    const Vec3f ray_direction_sph = Vec3f(1.0, pi/2.0 + (position[1]-0.5)*fov_[0], (position[0]-0.5)*fov_[1]).to_xyz_offset(direction_, horizontal, vertical); // 0, y, x

    const Ray_t focus_ray = Ray_t(origin_, ray_direction_sph, Vec3f(), Vec3f(1.0), medium_list_);

    if (scene->intersect(focus_ray, t, uv) == nullptr) {
        t = 1000000.0;
    }
    focus(t);
}