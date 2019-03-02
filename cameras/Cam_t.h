#ifndef CAM_T_H
#define CAM_T_H

#include "Camera_t.h"
#include "Vec3f.h"
#include <string>
#include <list>

class TransformMatrix_t;
class Skybox_t;
class Scene_t;
class Medium_t;
class ImgBuffer_t;

class Cam_t : public Camera_t{
    public:
        Cam_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, double fov[2], unsigned int subpix[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind);
        ~Cam_t();

        ImgBuffer_t* image_;

        virtual void update();
        virtual void raytrace(Scene_t* scene) const;
        virtual void write(std::string file_name = "");
        virtual void show() const;
};
#endif