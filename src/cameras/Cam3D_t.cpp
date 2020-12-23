#include "cameras/Cam3D_t.h"
#include "cameras/Cam_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Skybox_t.h"
#include "entities/Scene_t.h"
#include "entities/Medium_t.h"
#include "entities/ImgBuffer_t.h"
#include "entities/RandomGenerator_t.h"

using APTracer::Entities::Vec3f;
using APTracer::Cameras::Cam3D_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;

Cam3D_t::Cam3D_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, std::array<double, 2> fov, std::array<unsigned int, 2> subpix, ImgBuffer_t* image, ImgBuffer_t* image_L, ImgBuffer_t* image_R, double eye_dist, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double gammaind) :
    Camera_t(transformation, filename, up, fov, subpix, std::move(medium_list), skybox, max_bounces, gammaind), 
    image_(image), unif_(0.0, 1.0), eye_dist_(eye_dist/2.0), focal_length_(focal_length), focal_length_buffer_(focal_length) {

    std::string filename_S, filename_L, filename_R;

    const size_t point = filename.find_last_of('.');

    if (point != std::string::npos) {
        filename_L = filename.substr(0, point) + "_L" + filename.substr(point);
        filename_R = filename.substr(0, point) + "_R" + filename.substr(point);
        filename_S = filename;
    }
    else{
        filename_L = filename + "_L.png";
        filename_R = filename + "_R.png";
        filename_S = filename;
    }

    camera_L_ = new Cam_t(transformation, filename_L, up_, fov_, subpix_, image_L, medium_list_, skybox_, max_bounces_, gammaind_);
    camera_R_ = new Cam_t(transformation, filename_R, up_, fov_, subpix_, image_R, medium_list_, skybox_, max_bounces_, gammaind_);

    filename_ = filename_S;

    const Vec3f horizontal = direction_.cross(up).normalize_inplace();

    camera_L_->origin_ = horizontal * -eye_dist_ + origin_;
    camera_R_->origin_ = horizontal * eye_dist_ + origin_;
    camera_L_->direction_ = (direction_ * focal_length_ + horizontal * eye_dist_).normalize_inplace();
    camera_R_->direction_ = (direction_ * focal_length_ - horizontal * eye_dist_).normalize_inplace();
}

Cam3D_t::~Cam3D_t() {
    delete camera_L_;
    delete camera_R_;
}

void Cam3D_t::update() {
    origin_ = transformation_->multVec(Vec3f());
    direction_ = transformation_->transformDir().multDir(Vec3f(0.0, 1.0, 0.0));
    focal_length_ = focal_length_buffer_;
    up_ = up_buffer_;

    camera_L_->up_ = up_;
    camera_R_->up_ = up_;

    const Vec3f horizontal = direction_.cross(up_);
    camera_L_->origin_ = horizontal * -eye_dist_ + origin_;
    camera_R_->origin_ = horizontal * eye_dist_ + origin_;
    camera_L_->direction_ = (direction_ * focal_length_ + horizontal * eye_dist_).normalize_inplace();
    camera_R_->direction_ = (direction_ * focal_length_ - horizontal * eye_dist_).normalize_inplace();
}

void Cam3D_t::raytrace(const Scene_t* scene) {
    camera_L_->raytrace(scene);
    camera_R_->raytrace(scene);

    image_->update();
    for (unsigned int j = 0; j < image_->size_y_; j++) {
        for (unsigned int i = 0; i < image_->size_x_; i++) {
            image_->set(Vec3f(camera_L_->image_->img_[j*camera_L_->image_->size_x_ + i][0], camera_R_->image_->img_[j*camera_L_->image_->size_x_ + i][1], camera_R_->image_->img_[j*camera_L_->image_->size_x_ + i][2]), i, j);
        }
    }
}

void Cam3D_t::write(std::string file_name /*= ""*/) {
    std::string filename_S, filename_L, filename_R;

    if (file_name.empty()) {
        filename_L = "";
        filename_R = "";
        filename_S = filename_;
    }
    else{
        const size_t point = file_name.find_last_of('.');
        if (point != std::string::npos) {
            filename_L = file_name.substr(0, point) + "_L" + file_name.substr(point);
            filename_R = file_name.substr(0, point) + "_R" + file_name.substr(point);
            filename_S = file_name;
        }
        else{
            filename_L = file_name + "_L.png";
            filename_R = file_name + "_R.png";
            filename_S = file_name;
        }
    }
    camera_L_->write(filename_L);
    camera_R_->write(filename_R);
    image_->write(filename_S);
}

void Cam3D_t::show() const {
    // What to do here?
}

void Cam3D_t::reset() {
    camera_L_->reset();
    camera_R_->reset();
    image_->reset();
}

void Cam3D_t::focus(double focus_distance) {
    focal_length_buffer_ = focus_distance;
}

void Cam3D_t::autoFocus(const Scene_t* scene, const double (&position)[2]) {
    double t = std::numeric_limits<double>::infinity();
    double uv[2];

    const Vec3f ray_direction_sph = (direction_.get_sph() + Vec3f(0, (position[1]-0.5)*fov_[0], (position[0]-0.5)*-fov_[1])).to_xyz(); // 0, y, x

    const Ray_t focus_ray = Ray_t(origin_, ray_direction_sph, Vec3f(), Vec3f(1.0), medium_list_);

    if (scene->intersect(focus_ray, t, uv) == nullptr) {
        t = 1000000.0;
    }
    focus(t);
}