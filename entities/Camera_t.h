#ifndef CAMERA_T_H
#define CAMERA_T_H

#include "Scene_t.h"
#include "Vec3f.h"

class Camera_t{
    public:
        Camera_t(){};
        Camera_t();

        virtual void update() = 0;
        virtual void raytrace(Scene_t scene) const = 0;
        virtual void accumulate(Scene_t scene, unsigned int n_iter = 0) const;
        virtual void focus(double focus_distance){};
        virtual void autoFocus(Scene_t scene, double position[2]){};
        virtual void setUp(Vec3f new_up);
};

#endif