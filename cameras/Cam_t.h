#ifndef CAM_T_H
#define CAM_T_H

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

class Cam_t final : public Camera_t{
    public:
        Cam_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind);
        virtual ~Cam_t() final;

        ImgBuffer_t* image_;
        std::uniform_real_distribution<double> unif_;

        virtual void update() final;
        virtual void raytrace(const Scene_t* scene) final;
        virtual void write(std::string file_name = "") final;
        virtual void show() const final;
        virtual void reset() final;
};
#endif