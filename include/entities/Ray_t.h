#ifndef APTRACER_RAY_T_H
#define APTRACER_RAY_T_H

#include "entities/Vec3f.h"
#include <list>

namespace APTracer { namespace Entities {
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
            /**
             * @brief Construct a new Ray_t object.
             * 
             * @param[in] origin Initial origin of the ray.
             * @param[in] direction Initial direction of the ray.
             * @param[in] colour Initial colour of the ray. Usually [0 0 0] and is increased by hitting emissive objects.
             * @param[in] mask Initial part of the ray not yet absorbed. Is multiplied with the contribution of a light source to increment colour. Usually [1 1 1] and is decreased by being absorbed by materials.
             * @param[in] medium_list Initial list of materials through which the ray is travelling. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             */
            Ray_t(const Vec3f &origin, const Vec3f &direction, const Vec3f &colour, const Vec3f &mask, std::list<Medium_t*> medium_list);

            /**
             * @brief Construct a new Ray_t object with a  time.
             * 
             * @param[in] origin Initial origin of the ray.
             * @param[in] direction Initial direction of the ray.
             * @param[in] colour Initial colour of the ray. Usually [0 0 0] and is increased by hitting emissive objects.
             * @param[in] mask Initial part of the ray not yet absorbed. Is multiplied with the contribution of a light source to increment colour. Usually [1 1 1] and is decreased by being absorbed by materials.
             * @param[in] medium_list Initial list of materials through which the ray is travelling. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             * @param[in] time Time at which the ray is emitted. From 0 for exposure start to 1 for exposure end.
             */
            Ray_t(const Vec3f &origin, const Vec3f &direction, const Vec3f &colour, const Vec3f &mask, std::list<Medium_t*> medium_list, double time);
            
            /**
             * @brief Destroy the Ray_t object, nothing to be done here.
             * 
             */
            ~Ray_t();

            Vec3f origin_; /**< @brief Origin of the ray. Changed by materials on bounce.*/
            Vec3f direction_; /**< @brief Direction of the ray. Changed by materials on bounce.*/
            Vec3f colour_; /**< @brief Colour accumulated by the ray. Changed by emissive materials. Starts at [0 0 0].*/
            Vec3f mask_; /**< @brief Part of the ray not yet absorbed by materials. Multiplies the emission of materials to set colour. Starts at [1 1 1], the color can't be changed once the mask reaches 0.*/     
            double dist_; /**< @brief Distance traveled by the ray since last bounce.*/
            std::list <Medium_t*> medium_list_; /**< @brief List of materials in which the ray travels. The first one is the current one.*/
            double time_; /**< @brief Time of emission of the ray, relative to exposure time. 0 for start of exposure to 1 for end.*/

            /**
             * @brief Intersects the ray with objects in the scene and bounces it on their material.
             * 
             * This is the main function to compute the colour of a ray. A ray is sent through the scene,
             * finding the closest object hit. Then, the ray is modified by this object's material.
             * This change can change ray direction, origin, colour and mask. This process is repeated
             * up to max_bounces times, or until no object is it, at which point the skybox is intersected.
             * The ray is also modified by its first medium using the scatter function. If it is scattered,
             * it won't be bounced on the hit object's material, as it intersects the medium instead of 
             * the object.
             * 
             * @param[in] scene Scene containing the objects the ray will intersect.
             * @param[in] max_bounces Upper bound of number of bounces. Number of bounces may be less if no object is hit or ray can't be illuminated anymore.
             * @param[in] skybox Skybox that will be intersected if no object is hit.
             */
            void raycast(const Scene_t* scene, unsigned int max_bounces, const Skybox_t* skybox);
            
            /**
             * @brief Adds a medium to a ray's list of mediums, accordint to the medium's prioriy.
             * 
             * Adds a medium to a ray's list of mediums. The input medium will be added before the first
             * medium from the list which has a priority equal or superior to the input medium's 
             * priority. If it becomes the first element of the list, it becomes the active medium.
             * 
             * @param[in] medium Medium to be added to list of mediums.
             */
            void add_to_mediums(Medium_t* medium);

            /**
             * @brief Removes the first instance of the input medium in the list of mediums.
             * 
             * Removes a medium from a ray's list of mediums. Will remove the first instance of the
             * medium. The first instance means the instance with the highest priority, so the
             * priority value closest to 0.
             * 
             * @param[in] medium Medium to be removed from the list of mediums.
             */
            void remove_from_mediums(Medium_t* medium);
    };
}}

#endif