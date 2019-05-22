#include "Cam3D_t.h"
#include "Cam_t.h"
#include "TransformMatrix_t.h"
#include "Skybox_t.h"
#include "Scene_t.h"
#include "Medium_t.h"
#include "ImgBuffer_t.h"
#include "RandomGenerator_t.h"

#define PI 3.141592653589793238463

Cam3D_t::Cam3D_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, ImgBuffer_t* image_L, ImgBuffer_t* image_R, double eye_dist, std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double gammaind) :
    Camera_t(transformation, filename, up, fov, subpix, medium_list, skybox, max_bounces, gammaind), 
    image_(image), unif_(0.0, 1.0), eye_dist_(eye_dist), focal_length_(focal_length), focal_length_buffer_(focal_length) {

    std::string filename_S, filename_L, filename_R;
    size_t point;
    Vec3f horizontal;

    point = filename.find_last_of(".");

    if (point != std::string::npos){
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

    horizontal = direction_.cross(up).normalize();

    camera_L_->origin_ = horizontal * -eye_dist_/2.0 + origin_;
    camera_R_->origin_ = horizontal * eye_dist_/2.0 + origin_;
    camera_L_->direction_ = (direction_ * focal_length_ + horizontal * eye_dist_/2.0).normalize();
    camera_R_->direction_ = (direction_ * focal_length_ - horizontal * eye_dist_/2.0).normalize();
}

Cam3D_t::~Cam3D_t() {
    delete camera_L_;
    delete camera_R_;
}

void Cam3D_t::update() {
    Vec3f horizontal;

    origin_ = transformation_->multVec(Vec3f(0.0, 0.0, 0.0));
    TransformMatrix_t transform_norm = transformation_->transformDir();
    direction_ = transform_norm.multDir(Vec3f(0.0, 1.0, 0.0));
    focal_length_ = focal_length_buffer_;
    up_ = up_buffer_;

    camera_L_->up_ = up_;
    camera_R_->up_ = up_;

    horizontal = direction_.cross(up_);
    camera_L_->origin_ = horizontal * -eye_dist_/2.0 + origin_;
    camera_R_->origin_ = horizontal * eye_dist_/2.0 + origin_;
    camera_L_->direction_ = (direction_ * focal_length_ + horizontal * eye_dist_/2.0).normalize();
    camera_R_->direction_ = (direction_ * focal_length_ - horizontal * eye_dist_/2.0).normalize();
}

void Cam3D_t::raytrace(const Scene_t* scene) {
    camera_L_->raytrace(scene);
    camera_R_->raytrace(scene);

    image_->update();
    for (unsigned int j = 0; j < image_->size_y_; j++){
        for (unsigned int i = 0; i < image_->size_x_; i++){
            image_->set(Vec3f(camera_L_->image_->img_[j][i][0], camera_R_->image_->img_[j][i][1], camera_R_->image_->img_[j][i][2]), i, j);
        }
    }
}

void Cam3D_t::write(std::string file_name /*= ""*/) {
    std::string filename_S, filename_L, filename_R;

    if (file_name.empty()){
        filename_L = "";
        filename_R = "";
        filename_S = filename_;
    }
    else{
        size_t point = file_name.find_last_of(".");
        if (point != std::string::npos){
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

void Cam3D_t::reset(){
    camera_L_->reset();
    camera_R_->reset();
    image_->reset();
}

void Cam3D_t::focus(double focus_distance){
    focal_length_buffer_ = focus_distance;
}

void Cam3D_t::autoFocus(const Scene_t* scene, const double (&position)[2]){
    Vec3f ray_direction_sph;
    Shape_t* hit_obj = nullptr;
    double t = std::numeric_limits<double>::infinity();
    double uv[2];

    ray_direction_sph = direction_.get_sph() + Vec3f(0, (position[1]-0.5)*fov_[0], (position[0]-0.5)*-fov_[1]); // 0, y, x

    Ray_t focus_ray = Ray_t(origin_, ray_direction_sph.to_xyz(), Vec3f(), Vec3f(1.0, 1.0, 1.0), medium_list_);

    scene->intersect(focus_ray, hit_obj, t, uv);

    if (t == std::numeric_limits<double>::infinity()){
        t = 1000000.0;
    }
    focus(t);
}