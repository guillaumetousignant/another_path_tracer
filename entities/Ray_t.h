#ifndef RAY_T_H
#define RAY_T_H

#include "Vec3f.h"
#include <list>

class Medium_t;
class Scene_t;
class Skybox_t;

/**
 * @brief The ray class contains colour data and can be intersected with geometry.
 * 
 * This is the main class of the program. It describes a ray with a position and a direction. 
 * It also has colour data. It can be intersected with shapes or any intersectable object.
 * It holds a list a mediums to figure out in what material it is.
 */
class Ray_t{
    public:
        Ray_t(const Vec3f &origin, const Vec3f &direction, const Vec3f &colour, const Vec3f &mask, const std::list<Medium_t*> &medium_list, double time = 1.0);
        ~Ray_t();

        Vec3f origin_; /** @brief Origin of the ray. Changed by materials on bounce.*/
        Vec3f direction_; /** @brief Direction of the ray. Changed by materials on bounce.*/
        Vec3f colour_; /** @brief Colour accumulated by the ray. Changed by emissive materials. Starts at [0 0 0].*/
        Vec3f mask_; /** @brief Part of the ray not yet absorbed by materials. Multiplies the emission of materials to set colour. Starts at [1 1 1], the color can't be changed once the mask reaches 0.*/     
        double dist_; /** @brief Distance traveled by the ray since last bounce.*/
        std::list <Medium_t*> medium_list_; /** @brief List of materials in which the ray travels. The first one is the current one.*/
        double time_; /** @brief Time of emission of the ray, relative to exposure time. 0 for start of exposure to 1 for end.*/

        void raycast(const Scene_t* scene, unsigned int max_bounces, const Skybox_t* skybox);
        void add_to_mediums(Medium_t* medium);
        void remove_from_mediums(Medium_t* medium);
};

#endif