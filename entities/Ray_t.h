#ifndef RAY_T_H
#define RAY_T_H

#include "Vec3f.h"
//#include "Medium_t.h"
#include <list>
//#include "Scene_t.h"
//#include "Skybox_t.h"

class Medium_t;
class Scene_t;
class Skybox_t;

class Ray_t{
    public:
        Ray_t(const Vec3f &origin, const Vec3f &direction, const Vec3f &colour, const Vec3f &mask, const std::list<Medium_t*> &medium_list, double time = 1.0); // Make constructor take time as opt input
        ~Ray_t();

        Vec3f origin_;
        Vec3f direction_;
        Vec3f colour_;
        Vec3f mask_;        
        double dist_;
        std::list <Medium_t*> medium_list_;
        double time_;

        void raycast(const Scene_t* scene, unsigned int max_bounces, const Skybox_t* skybox);
        void add_to_mediums(Medium_t* medium);
        void remove_from_mediums(Medium_t* medium);
};

#endif