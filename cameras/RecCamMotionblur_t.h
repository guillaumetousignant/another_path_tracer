#ifndef RECCAMMOTIONBLUR_T_H
#define RECCAMMOTIONBLUR_T_H

#include "RecCam_t.h"
#include "Vec3f.h"
#include <string>
#include <list>
#include <random>

class TransformMatrix_t;
class Skybox_t;
class Scene_t;
class Medium_t;
class ImgBuffer_t;

class RecCamMotionblur_t : public RecCam_t{
    public:
        RecCamMotionblur_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double (&time)[2], double gammaind);
        ~RecCamMotionblur_t();

        Vec3f direction_last_;
        Vec3f origin_last_;
        double time_[2];
        Vec3f up_last_;

        virtual void update();
        virtual void raytrace(const Scene_t* scene);
};
#endif