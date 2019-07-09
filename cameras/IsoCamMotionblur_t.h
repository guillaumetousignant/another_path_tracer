#ifndef ISOCAMMOTIONBLUR_T_H
#define ISOCAMMOTIONBLUR_T_H

#include "IsoCam_t.h"
#include "Vec3f.h"
#include <string>
#include <list>
#include <random>

class TransformMatrix_t;
class Skybox_t;
class Scene_t;
class Medium_t;
class ImgBuffer_t;

class IsoCamMotionblur_t : public IsoCam_t{
    public:
        IsoCamMotionblur_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double (&time)[2], double gammaind);
        virtual ~IsoCamMotionblur_t();

        Vec3f direction_last_;
        Vec3f origin_last_;
        double time_[2];
        Vec3f up_last_;

        virtual void update();
        virtual void raytrace(const Scene_t* scene);
};
#endif