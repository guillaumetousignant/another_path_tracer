#ifndef CAM3D_T_H
#define CAM3D_T_H

#include "Camera_t.h"
#include "Vec3f.h"
#include <string>
#include <list>
#include <random>

using APTracer::Entities::Camera_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::ImgBuffer_t;

class TransformMatrix_t;
class Skybox_t;
class Scene_t;
class Medium_t;
class ImgBuffer_t;

namespace APTracer::Cameras {
    class Cam_t;
    
    class Cam3D_t  final: public Camera_t{
        public:
            Cam3D_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, ImgBuffer_t* image_L, ImgBuffer_t* image_R, double eye_dist, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double gammaind);
            virtual ~Cam3D_t() final;

            ImgBuffer_t* image_;
            std::uniform_real_distribution<double> unif_;
            double eye_dist_;
            double focal_length_;
            double focal_length_buffer_;
            Cam_t* camera_L_;
            Cam_t* camera_R_;

            virtual void update() final;
            virtual void raytrace(const Scene_t* scene) final;
            virtual void write(std::string file_name = "") final;
            virtual void show() const final;
            virtual void reset() final;
            virtual void focus(double focus_distance) final;
            virtual void autoFocus(const Scene_t* scene, const double (&position)[2]) final;
    };
}
#endif