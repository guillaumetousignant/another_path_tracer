#ifndef CAMERA_T_H
#define CAMERA_T_H

//#include "Scene_t.h"
#include "Vec3f.h"
#include <string>
#include <list>
//#include "TransformMatrix_t.h"
//#include "Skybox_t.h"
//#include "Medium_t.h"

class TransformMatrix_t;
class Skybox_t;
class Scene_t;
class Medium_t;

class Camera_t{
    public:
        Camera_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], std::list<Medium_t*> &medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind);
        Camera_t(){};

        TransformMatrix_t* transformation_;
        std::string filename_;
        double fov_[2];
        unsigned int subpix_[2];
        std::list<Medium_t*> medium_list_;
        Skybox_t* skybox_;        
        unsigned int max_bounces_;
        Vec3f direction_;
        Vec3f origin_;
        double gammaind_;
        Vec3f up_;
        Vec3f up_buffer_;     
        
        virtual void update() = 0;
        virtual void raytrace(const Scene_t* scene) = 0;
        virtual void accumulate(const Scene_t* scene, unsigned int n_iter = 1000000000);
        virtual void accumulateWrite(const Scene_t* scene, unsigned int n_iter = 1000000000, unsigned int interval = 1);
        virtual void focus(double focus_distance){};
        virtual void autoFocus(const Scene_t* scene, const double (&position)[2]){};
        virtual void setUp(Vec3f &new_up);
        virtual void write(std::string file_name = "") = 0;
        virtual void show() const = 0;
        virtual void reset() = 0;
};

#endif