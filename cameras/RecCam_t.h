#ifndef RECCAM_T_H
#define RECCAM_T_H

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

class RecCam_t : public Camera_t{
    public:
        RecCam_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind);
        ~RecCam_t();

        ImgBuffer_t* image_;
        std::uniform_real_distribution<double> unif_;

        virtual void update();
        virtual void raytrace(const Scene_t* scene);
        virtual void write(std::string file_name = "");
        virtual void show() const;
        virtual void reset();
};
#endif