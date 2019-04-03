#ifndef CAMMOTIONBLURAPERTURE_T_H
#define CAMMOTIONBLURAPERTURE_T_H

#include "CamMotionblur_t.h"
#include "Vec3f.h"
#include <string>
#include <list>
#include <random>

class TransformMatrix_t;
class Skybox_t;
class Scene_t;
class Medium_t;
class ImgBuffer_t;

class CamMotionblurAperture_t : public CamMotionblur_t{
    public:
        CamMotionblurAperture_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double aperture, double (&time)[2], double gammaind);
        ~CamMotionblurAperture_t();

        double focal_length_;
        double focal_length_last_;
        double aperture_;
        double focal_length_buffer_;

        virtual void update();
        virtual void raytrace(const Scene_t* scene);        
        virtual void focus(double focus_distance);
        virtual void autoFocus(const Scene_t* scene, double position[2]);
};
#endif