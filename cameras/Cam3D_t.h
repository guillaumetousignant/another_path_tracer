#ifndef CAM3D_T_H
#define CAM3D_T_H

#include "Camera_t.h"
#include "Vec3f.h"
#include <string>
#include <list>
#include <random>

class TransformMatrix_t;
class Skybox_t;
class Scene_t;
class Medium_t;
class ImgBuffer_t;

class Cam3D_t : public Camera_t{
    public:
        Cam3D_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, ImgBuffer_t* image_L, ImgBuffer_t* image_R, double eye_dist, std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double gammaind);
        ~Cam3D_t();

        ImgBuffer_t* image_;
        std::uniform_real_distribution<double> unif_;
        double eye_dist_;
        double focal_length_;
        double focal_length_buffer_;
        Camera_t* camera_L_;
        Camera_t* camera_R_;

        virtual void update();
        virtual void raytrace(const Scene_t* scene);
        virtual void write(std::string file_name = "");
        virtual void show() const;
        virtual void reset();
        virtual void focus(double focus_distance);
        virtual void autoFocus(const Scene_t* scene, const double (&position)[2]);
};
#endif