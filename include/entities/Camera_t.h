#ifndef APTRACER_CAMERA_T_H
#define APTRACER_CAMERA_T_H

#include "entities/Vec3f.h"
#include <string>
#include <list>
#include <array>

namespace APTracer { namespace Entities {
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
            Camera_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, std::array<double, 2> fov, std::array<unsigned int, 2> subpix, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind);
            
            /**
             * @brief Destroy the Camera_t object. Does nothing.
             */
            virtual ~Camera_t() {};

            TransformMatrix_t* transformation_; /**< @brief Transformation matrix used by the camera. Sets the camera's origin and direction when created and updated.*/
            std::string filename_; /**< @brief Filename used by the camera when saving an image. Not used by all cameras, and some might modify it or save multiple pictures with variations of the name.*/
            std::array<double, 2> fov_; /**< @brief Array containing field of view of camera [vertical, horizontal]. Will have different units depending on the projection used.*/
            std::array<unsigned int, 2> subpix_; /**< @brief Array containing the number of subpixels per pixel, [vertical, horizontal], for antialiasing purposes. Not very useful because sample jittering when there are multiple samples per pixel removes aliasing. May be useful when there are few samples per pixel and location of the samples mush be controlled.*/
            std::list<Medium_t*> medium_list_; /**< @brief List of materials in which the camera is placed. Active medium is first element. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).*/
            Skybox_t* skybox_; /**< @brief Skybox that will be intersected if no shape is hit by the rays.*/    
            unsigned int max_bounces_; /**< @brief Maximum intersections with shapes and bounces on materials a ray can do before it is terminated. Actual number may be less.*/
            Vec3f direction_; /**< @brief Direction in which the camera points. Changed by modifying the camera's transformation matrix.*/
            Vec3f origin_; /**< @brief Position of the camera. Changed by modifying the camera's transformation matrix.*/
            double gammaind_; /**< @brief Gamma of the saved picture. A value of 1 should be used for usual cases.*/
            Vec3f up_; /**< @brief Vector pointing up. Used to set the roll of the camera. Changed by setUp.*/
            Vec3f up_buffer_; /**< @brief Stores the up vector until the camera is updated.*/   
            
            /**
             * @brief Updates the camera's members.
             * 
             * This is used to set the new direction, origin, up, and other variables. Should be called once per frame, before rendering. This is how the changes to the transformation matrix and functions like setUp take effect.
             */
            virtual void update() = 0;

            /**
             * @brief Sends rays through the scene, to generate an image.
             * 
             * The camera will generate rays according to its projection, and cast them through the provided scene. The resulting colour can be written to an image buffer. This will generate one image.
             * 
             * @param scene Scene that will be used to find what each ray hits.
             */
            virtual void raytrace(const Scene_t* scene) = 0;

            /**
             * @brief Raytraces the scene multiple times to get more samples per pixel.
             * 
             * This function will raytrace the same scene multiple times in order to accumulate more samples. This will reduce noise.
             * 
             * @param scene Scene that will be used to find what each ray hits.
             * @param n_iter Number of times the scene will be raytraced. Defaults to 1000000000.
             */
            void accumulate(const Scene_t* scene, unsigned int n_iter = 1000000000);

            /**
             * @brief Raytraces the scene multiple times to get more samples per pixel, saving the image every so often.
             * 
             * This function will raytrace the same scene multiple times in order to accumulate more samples. This will reduce noise. It will also save the generated image at an interval. 
             * 
             * @param scene Scene that will be used to find what each ray hits.
             * @param n_iter Number of times the scene will be raytraced. Defaults to 1000000000.
             * @param interval Saves the image every x frames by calling write(). Defaults to 1.
             */
            void accumulateWrite(const Scene_t* scene, unsigned int n_iter = 1000000000, unsigned int interval = 1);

            /**
             * @brief Sets the focal length of the camera to a specific distance.
             * 
             * This is used to set the focal plane distance of the camera to a given value. Defaults to doing nothing, as cameras use the pinhole model by default, where everything is in focus.
             * The focal plane's shape will vary based on the projection used.
             * 
             * @param focus_distance How far will the focal plane be.
             */
            virtual void focus(double focus_distance) {};

            /**
             * @brief Sets the focal length of the camera to put an object in focus.
             * 
             * The scene will be intersected at a specific position of the frame, and the hit distance returned will be used to set the focal length. This effectively puts that object in focus.
             * defaults to doing nothing, as cameras use the pinhole model by default, where everything is in focus. Will also intersect invisible objects, and won't propagate through mirrors and such.
             * The focal plane's shape will vary based on the projection used.
             * 
             * @param scene Scene that will be used to find what object the ray hits and its distance.
             * @param position Where in the frame will the ray be sent. [horizontal, vertical], both from 0 to 1, starting from bottom left.
             */
            virtual void autoFocus(const Scene_t* scene, const double (&position)[2]) {};

            /**
             * @brief Set the up vector of the camera.
             * 
             * The actual up will be changed on next update.
             * 
             * @param new_up New up vector for the camera.
             */
            void setUp(Vec3f &new_up);

            /**
             * @brief Writes the image buffer to disk with the provided name, or the camera's filename if empty.
             * 
             * This will write the camera's image to disk or otherwise. It uses the input name, or the camera's filename_ if called with "". The actual name may be prefixed or modified, especially if 
             * multiple images are written.
             * 
             * @param file_name Filename used to write the images. If "", uses the camera's filename_. May be changed or prefixed.
             */
            virtual void write(std::string file_name = "") = 0;

            /**
             * @brief Shows the image on screen. Currently not implemented. Use ImgBufferOpenGL_t instead.
             */
            virtual void show() const = 0;

            /**
             * @brief Resets the camera's image buffer(s), for when the scene or camera has changed.
             * 
             * This will discard all accumulated samples and start accumulation from scratch.
             */
            virtual void reset() = 0;
    };
}}

#endif