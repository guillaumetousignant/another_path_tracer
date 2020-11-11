#ifndef CAMMOTIONBLURAPERTURE_T_H
#define CAMMOTIONBLURAPERTURE_T_H

#include "entities/Camera_t.h"
#include "entities/Vec3f.h"
#include <string>
#include <list>
#include <random>

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Skybox_t;
    class Scene_t;
    class Medium_t;
    class ImgBuffer_t;
}}

using APTracer::Entities::Camera_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::ImgBuffer_t;

namespace APTracer { namespace Cameras {

    /**
     * @brief The cam motionblur aperture class describes a camera that uses a spherical projection, a shutter for motion blur, and has an aperture for depth of field.
     * 
     * It uses a spherical projection [r, theta, phi], where rays are equally spaced 
     * in the theta and phi directions. This reduces warping at higher fields of view,
     * and permits the usage of arbitrarily high fields of view.
     * This camera creates rays with a random time between the last state and current 
     * state of the scene. This enables motion blur, as objects can use this to interpolate
     * their state at the ray's time. The rays are also created with the interpolated 
     * camera state.
     * This camera creates rays on a disk to simulate the aperture of a real camera for depth of field.
     * It has a focal length, so all objects at that distance will be in the focal plane,
     * and other objects will be out of focus. The shape of the focal plane is a sphere
     * at focal_length_ from the origin.
     * This camera stores the result from its rays in a single image buffer.
     */
    class CamMotionblurAperture_t final : public Camera_t{
        public:
            /**
             * @brief Construct a new CamMotionblurAperture_t object. Most arguments are passed to the Camera_t constructor.
             * 
             * @param transformation Transformation matrix used by the camera. Sets the camera's origin and direction when created and updated.
             * @param filename Filename used by the camera when saving an image.
             * @param up Vector pointing up. Used to set the roll of the camera.
             * @param fov Array containing field of view of camera in radians [vertical, horizontal].
             * @param subpix Array containing the number of subpixels per pixel, [vertical, horizontal], for antialiasing purposes. Usually [1, 1].
             * @param image Image buffer into which the resulting image will be stored.
             * @param medium_list Initial list of materials in which the camera is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             * @param skybox Skybox that will be intersected if no shape is hit by the rays.
             * @param max_bounces Maximum intersections with shapes and bounces on materials a ray can do before it is terminated. Actual number may be less.
             * @param focal_length Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus.
             * @param aperture Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.
             * @param time Opening and closing time of the shutter. [open, close], from 0 to 1, where 0 is last state and current state. Rays are created at a time in this interval. Enables motion blur.
             * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
             */
            CamMotionblurAperture_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double aperture, double (&time)[2], double gammaind);
            
            /**
             * @brief Destroy the CamMotionblurAperture_t object. Does nothing.
             */
            virtual ~CamMotionblurAperture_t() final;

            ImgBuffer_t* image_; /**< @brief Image buffer into which the image is stored.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            Vec3f direction_last_; /**< @brief Direction in which the camera points before last updated. Used for motion blur.*/
            Vec3f origin_last_; /**< @brief Position of the camera before last update. Used for motion blur.*/
            double time_[2]; /**< @brief Opening and closing time of the shutter. [open, close], from 0 to 1, where 0 is last state and current state. Rays are created at a time in this interval. Enables motion blur.*/
            Vec3f up_last_; /**< @brief Vector pointing up before last update. Used for motion blur.*/
            double focal_length_; /**< @brief Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus. The focal plane has the shape of a sphere with this radius.*/
            double focal_length_last_; /**< @brief Distance of the focal plane to the camera origin before last update. Used for motion blur.*/
            double aperture_; /**< @brief Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.*/
            double focal_length_buffer_; /**< @brief Focal length to be modified between updates. Its value is given to the real focal length on update.*/

            /**
             * @brief Updates the camera's members.
             * 
             * This is used to set the new direction, origin, up vector, and focal length. Should be called once per frame, before rendering. 
             * This is how the changes to the transformation matrix and functions like setUp take effect. It sets focal_length_ to focal_length_buffer_,
             * so it is only modified once per frame. This is needed to correctly interpolate focal length for motion blur.
             * Stores the previous state in the _last variables, so that state can be interpolated according to time.
             */
            virtual void update() final;

            /**
             * @brief Sends rays through the scene, to generate an image.
             * 
             * The camera will generate rays according to a spherical projection, and cast them through the provided scene. 
             * The resulting colour is written to the image buffer. This will generate one image. Rays are created with a 
             * time inside the time_ interval, to simulate a shutter and enable motion blur. Rays are created on a disk to 
             * simulate an aperture and depth of field.
             * 
             * @param scene Scene that will be used to find what each ray hits.
             */
            virtual void raytrace(const Scene_t* scene) final;

            /**
             * @brief Sets the focal length of the camera.
             * 
             * The focal length will be changed on next update along with other members.
             * All objects at that distance will be in the focal plane, and other objects will be out of focus.
             * The focal plane has the shape of a sphere with this radius.
             * 
             * @param focus_distance New focal length of the camera.
             */
            virtual void focus(double focus_distance) final;

            /**
             * @brief Focusses the camera on a point in its field of view.
             * 
             * This will send a ray at the specified coordinates in image space and set the focal length as
             * the length to its first intersection. The coordinates are [horizontal, vertical] from 0 to 1,
             * from left to right and bottom to top. Since the ray's first hit is used, this may behave 
             * unexpectedly with reflective surfaces, transparent surfaces, invisible surfaces, portals etc.
             * The ray has a a time value of 1, for the current state.
             * 
             * @param scene Scene to use to find the object to focus on.
             * @param position Image space coordinates on which to focus. [horizontal, vertical] from 0 to 1, from left to right and bottom to top.
             */
            virtual void autoFocus(const Scene_t* scene, const double (&position)[2]) final;

            /**
             * @brief Writes the image buffer to disk with the provided name, or the camera's filename if empty.
             * 
             * This will write the camera's image to disk. It uses the input name, or the camera's filename_ if called with "".
             * This calls the image buffer's write function. Directory must exist.
             * 
             * @param file_name Filename used to write the images. If "", uses the camera's filename_.
             */
            virtual void write(std::string file_name = "") final;

            /**
             * @brief Shows the image on screen. Currently not implemented. Use ImgBufferOpenGL_t instead.
             */
            virtual void show() const final;

            /**
             * @brief Resets the camera's image buffer, for when the scene or camera has changed.
             * 
             * This will discard all accumulated samples and start accumulation from scratch. Calls the image buffer's
             * reset function.
             */
            virtual void reset() final;
    };
}}
#endif