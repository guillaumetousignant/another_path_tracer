#ifndef CAMERA_T_H
#define CAMERA_T_H

#include "Vec3f.h"
#include <string>
#include <list>

class TransformMatrix_t;
class Skybox_t;
class Scene_t;
class Medium_t;

/**
 * @brief The camera class creates rays and casts them into the scene and stores the result in an image buffer.
 * 
 * This is an interface class that defines what all cameras must implement. It should handle
 * the creation of rays according to a projection, and cast them in a scene. It should then
 * store the colour of the ray in an image buffer. The interface has no image buffer, as
 * the needed image buffer(s) vary by configuration, or are even not needed for some specific
 * situations.
 */
class Camera_t{
    public:
        /**
         * @brief Construct a new Camera_t object. Never called directly.
         * 
         * This is the constructor for the base interface class for cameras. It sets some 
         * basic variables.
         * 
         * @param transformation Transformation matrix used by the camera. Sets the camera's origin and direction when created and updated.
         * @param filename Filename used by the camera when saving an image. Not used by all cameras, and some might modify it or save multiple pictures.
         * @param up Vector pointing up. Used to set the roll of the camera.
         * @param fov Array containing field of view of camera [vertical, horizontal].
         * @param subpix Array containing the number of subpixels per pixel, [vertical, horizontal], for antialiasing purposes. Usually [1, 1].
         * @param medium_list Initial list of materials in which the camera is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
         * @param skybox Skybox that will be intersected if no shape is hit by the rays.
         * @param max_bounces Maximum intersections with shapes and bounces on materials a ray can do before it is terminated. Actual number may be less.
         * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
         */
        Camera_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind);
        
        /**
         * @brief Destroy the Camera_t object. Does nothing.
         */
        virtual ~Camera_t(){};

        TransformMatrix_t* transformation_; /**< @brief Transformation matrix used by the camera. Sets the camera's origin and direction when created and updated.*/
        std::string filename_; /**< @brief Filename used by the camera when saving an image. Not used by all cameras, and some might modify it or save multiple pictures with variations of the name.*/
        double fov_[2]; /**< @brief Array containing field of view of camera [vertical, horizontal]. Will have different units depending on the projection used.*/
        unsigned int subpix_[2]; /**< @brief Array containing the number of subpixels per pixel, [vertical, horizontal], for antialiasing purposes. Not very useful because sample jittering when there are multiple samples per pixel removes aliasing. May be useful when there are few samples per pixel and location of the samples mush be controlled.*/
        std::list<Medium_t*> medium_list_; /**< @brief List of materials in which the camera is placed. Active mediumm is first element. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).*/
        Skybox_t* skybox_; /**< @brief */    
        unsigned int max_bounces_; /**< @brief */
        Vec3f direction_; /**< @brief */
        Vec3f origin_; /**< @brief */
        double gammaind_; /**< @brief */
        Vec3f up_; /**< @brief */
        Vec3f up_buffer_; /**< @brief */   
        
        virtual void update() = 0;
        virtual void raytrace(const Scene_t* scene) = 0;
        void accumulate(const Scene_t* scene, unsigned int n_iter = 1000000000);
        void accumulateWrite(const Scene_t* scene, unsigned int n_iter = 1000000000, unsigned int interval = 1);
        virtual void focus(double focus_distance){};
        virtual void autoFocus(const Scene_t* scene, const double (&position)[2]){};
        void setUp(Vec3f &new_up);
        virtual void write(std::string file_name = "") = 0;
        virtual void show() const = 0;
        virtual void reset() = 0;
};

#endif