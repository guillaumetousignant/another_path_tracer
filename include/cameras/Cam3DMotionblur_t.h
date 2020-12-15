#ifndef APTRACER_CAM3DMOTIONBLUR_T_H
#define APTRACER_CAM3DMOTIONBLUR_T_H

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
    class CamMotionblur_t;
    
    /**
     * @brief The cam 3D motionblur class describes a camera that uses a spherical projection, a shutter for motion blur and generates 3D images.
     * 
     * It uses a spherical projection [r, theta, phi], where rays are equally spaced 
     * in the theta and phi directions. This reduces warping at higher fields of view,
     * and permits the usage of arbitrarily high fields of view.
     * This camera creates rays with a random time between the last state and current 
     * state of the scene. This enables motion blur, as objects can use this to interpolate
     * their state at the ray's time. The rays are also created with the interpolated 
     * camera state.
     * This camera has no aperture.
     * This camera generates two images from offset positions and combines them to
     * create a 3D anaglyph image.
     */
    class Cam3DMotionblur_t  final: public Camera_t{
        public:
            /**
             * @brief Construct a new Cam3DMotionblur_t object. Creates two dependant cameras, one for each eye. Most arguments are passed to them.
             * 
             * @param transformation Transformation matrix used by the camera. Sets the camera's origin and direction when created and updated.
             * @param filename Filename used by the camera when saving an image. The 3D image will use it, and the left and right images will have _L and _R suffixes, respectively.
             * @param up Vector pointing up. Used to set the roll of the camera.
             * @param fov Array containing field of view of camera in radians [vertical, horizontal].
             * @param subpix Array containing the number of subpixels per pixel, [vertical, horizontal], for antialiasing purposes. Usually [1, 1].
             * @param image Image buffer into which the resulting 3D anaglyph image will be stored.
             * @param image_L Image buffer into which the resulting left eye image will be stored.
             * @param image_R Image buffer into which the resulting right eye image will be stored.
             * @param eye_dist Distance between the left and right eye cameras. Higher values will make the 3D effect stronger. 
             * @param medium_list Initial list of materials in which the camera is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             * @param skybox Skybox that will be intersected if no shape is hit by the rays.
             * @param max_bounces Maximum intersections with shapes and bounces on materials a ray can do before it is terminated. Actual number may be less.
             * @param focal_length Distance of the focal plane to the camera origin. Distance at which both eye cameras converge.
             * @param time Opening and closing time of the shutter. [open, close], from 0 to 1, where 0 is last state and current state. Rays are created at a time in this interval. Enables motion blur.
             * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
             */
            Cam3DMotionblur_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, ImgBuffer_t* image_L, ImgBuffer_t* image_R, double eye_dist, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double (&time)[2], double gammaind);
            
            /**
             * @brief Destroy the Cam3DMotionblur_t object, deleting the dependant cameras.
             */
            virtual ~Cam3DMotionblur_t() final;

            ImgBuffer_t* image_; /**< @brief Image buffer into which the resulting 3D anaglyph image is stored.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            double eye_dist_; /**< @brief Distance between the left and right eye cameras. Higher values will make the 3D effect stronger.*/
            double focal_length_; /**< @brief Distance of the focal plane to the camera origin. Distance at which both eye cameras converge.*/
            double focal_length_buffer_; /**< @brief Focal length to be modified between updates. Its value is given to the real focal length on update.*/

            Vec3f direction_last_; /**< @brief Direction in which the camera points before last update. Used for motion blur.*/
            Vec3f origin_last_; /**< @brief Position of the camera before last update. Used for motion blur.*/
            double focal_length_last_; /**< @brief Distance of the focal plane to the camera origin before last update. Used for motion blur.*/
            double time_[2]; /**< @brief Opening and closing time of the shutter. [open, close], from 0 to 1, where 0 is last state and current state. Rays are created at a time in this interval. Enables motion blur.*/
            Vec3f up_last_; /**< @brief Vector pointing up before last update. Used for motion blur.*/

            CamMotionblur_t* camera_L_; /**< @brief Left eye camera. Managed by the 3D camera, do not update directly.*/
            CamMotionblur_t* camera_R_; /**< @brief Right eye camera. Managed by the 3D camera, do not update directly.*/

            /**
             * @brief Updates the camera's members and the dependant cameras.
             * 
             * This is used to set the new direction, origin, up vector, and focal length. Should be called once per frame, before rendering. 
             * This is how the changes to the transformation matrix and functions like setUp take effect.
             * Stores the previous state in the _last variables, so that state can be interpolated according to time.
             * The left and right eye cameras are also updated directly.
             */
            virtual void update() final;

            /**
             * @brief Sends rays through the scene, to generate an image.
             * 
             * Both the left and right eye cameras will generate rays according to their projection, and cast them through the provided scene.
             * The resulting colour is written to the image buffer. This will generate one image per camera.
             * Then, the images are combined into the 3D camera's image buffer, to create a 3D anaglyph image.
             * Rays are created with a time inside the time_ interval, to simulate a shutter and enable motion blur.
             * 
             * @param scene Scene that will be used to find what each ray hits.
             */
            virtual void raytrace(const Scene_t* scene) final;

            /**
             * @brief Writes the image buffer and the left and right eye images to disk with the provided name, or the camera's filename if empty.
             * 
             * This will write the camera's image to disk. It uses the input name, or the camera's filename_ if called with "".
             * This calls the image buffer's write function. Directory must exist.
             * The 3D image will use the file name, and the left and right images will have _L and _R suffixes, respectively.
             * 
             * @param file_name Filename used to write the 3D image and the left and right images will have _L and _R suffixes. If "", uses the camera's filename_.
             */
            virtual void write(std::string file_name = "") final;

            /**
             * @brief Shows the image on screen. Currently not implemented. Use ImgBufferOpenGL_t instead.
             */
            virtual void show() const final;

            /**
             * @brief Resets the camera's and left and right cameras' image buffers, for when the scene or camera has changed.
             * 
             * This will discard all accumulated samples and start accumulation from scratch. Calls the image buffers'
             * reset functions.
             */
            virtual void reset() final;

            /**
             * @brief Sets the focal length of the camera, the distance at which both eye cameras converge.
             * 
             * The focal length will be changed on next update along with other members.
             * Distance at which both eye cameras converge, objects away from that distance will be out of focus.
             * 
             * @param focus_distance New focal length of the camera. Distance at which both eye cameras converge.
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
             * The ray has its origin between the two eye cameras.
             * 
             * @param scene Scene to use to find the object to focus on.
             * @param position Image space coordinates on which to focus. [horizontal, vertical] from 0 to 1, from left to right and bottom to top.
             */
            virtual void autoFocus(const Scene_t* scene, const double (&position)[2]) final;
    };
}}
#endif