#ifndef APTRACER_CAMERAS_FISHCAMMOTIONBLURAPERTURE_T_H
#define APTRACER_CAMERAS_FISHCAMMOTIONBLURAPERTURE_T_H

#include "entities/Camera_t.h"
#include "entities/Vec3f.h"
#include <list>
#include <random>
#include <string>

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Skybox_t;
    class Scene_t;
    class Medium_t;
    class ImgBuffer_t;
}}

using APTracer::Entities::Camera_t;
using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Vec3f;

namespace APTracer { namespace Cameras {

    /**
     * @brief The fish cam motionblur aperture class describes a fisheye camera that uses an equidistant projection, a shutter for motion blur, and has an aperture for depth of field.
     *
     * It uses an equidistant projection [theta, phi], where the distance from the center on the
     * image plane is directly proportional to the zenith angle, between the ray and the center
     * of the lens. This enables extreme field of views, at the cost of straight lines being
     * distorted if they don't go through the center of the image. Angles are not distorted
     * as with rectangular projections.
     * This camera creates rays with a random time between the last state and current
     * state of the scene. This enables motion blur, as objects can use this to interpolate
     * their state at the ray's time. The rays are also created with the interpolated
     * camera state.
     * This camera creates rays on a disk to simulate the aperture of a real camera for depth of field.
     * It has a focus distance, so all objects at that distance will be in the focal plane,
     * and other objects will be out of focus. The shape of the focal plane is a sphere
     * of focus_distance_ radius centered on the origin.
     * This camera stores the result from its rays in a single image buffer.
     */
    class FishCamMotionblurAperture_t final : public Camera_t {
        public:
            /**
             * @brief Construct a new FishCamMotionblurAperture_t object. Most arguments are passed to the Camera_t constructor.
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
             * @param focus_distance Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus.
             * @param aperture Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.
             * @param time Opening and closing time of the shutter. [open, close], from 0 to 1, where 0 is last state and current state. Rays are created at a time in this interval. Enables motion
             * blur.
             * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
             */
            FishCamMotionblurAperture_t(TransformMatrix_t* transformation,
                                        const std::string& filename,
                                        Vec3f up,
                                        std::array<double, 2> fov,
                                        std::array<unsigned int, 2> subpix,
                                        ImgBuffer_t* image,
                                        std::list<Medium_t*> medium_list,
                                        Skybox_t* skybox,
                                        unsigned int max_bounces,
                                        double focus_distance,
                                        double aperture,
                                        std::array<double, 2> time,
                                        double gammaind);

            ImgBuffer_t* image_; /**< @brief Image buffer into which the image is stored.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            Vec3f direction_last_; /**< @brief Direction in which the camera points before last update. Used for motion blur.*/
            Vec3f origin_last_; /**< @brief Position of the camera before last update. Used for motion blur.*/
            std::array<double, 2> time_; /**< @brief Opening and closing time of the shutter. [open, close], from 0 to 1, where 0 is last state and current state. Rays are created at a time in this
                                            interval. Enables motion blur.*/
            Vec3f up_last_; /**< @brief Vector pointing up before last update. Used for motion blur.*/
            double focus_distance_; /**< @brief Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus. The focal plane has the shape of a sphere with
                                       this radius.*/
            double focus_distance_last_; /**< @brief Distance of the focal plane to the camera origin before last update. Used for motion blur.*/
            double aperture_; /**< @brief Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.*/
            double focus_distance_buffer_; /**< @brief Focus distance to be modified between updates. Its value is given to the real focus distance on update.*/
            std::array<double, 2> fov_last_; /**< @brief Field of view before last update. Used for motion blur.*/

            /**
             * @brief Updates the camera's members.
             *
             * This is used to set the new direction, origin, up vector, and focus distance. Should be called once per frame, before rendering.
             * This is how the changes to the transformation matrix and functions like setUp take effect. It sets focus_distance_ to focus_distance_buffer_,
             * so it is only modified once per frame. This is needed to correctly interpolate focus distance for motion blur.
             * Stores the previous state in the _last variables, so that state can be interpolated according to time.
             */
            virtual auto update() -> void final;

            /**
             * @brief Sends rays through the scene, to generate an image.
             *
             * The camera will generate rays according to an equidistant projection, and cast them through the provided scene.
             * The resulting colour is written to the image buffer. This will generate one image. Rays are created with a
             * time inside the time_ interval, to simulate a shutter and enable motion blur. Rays are created on a disk to
             * simulate an aperture and depth of field.
             *
             * @param scene Scene that will be used to find what each ray hits.
             */
            virtual auto raytrace(const Scene_t* scene) -> void final;

            /**
             * @brief Sets the focus distance of the camera.
             *
             * The focus distance will be changed on next update along with other members.
             * All objects at that distance will be in the focal plane, and other objects will be out of focus.
             * The focal plane has the shape of a sphere with this radius.
             *
             * @param focus_distance New focus distance of the camera.
             */
            virtual auto focus(double focus_distance) -> void final;

            /**
             * @brief Focusses the camera on a point in its field of view.
             *
             * This will send a ray at the specified coordinates in image space and set the focus distance as
             * the length to its first intersection. The coordinates are [horizontal, vertical] from 0 to 1,
             * from left to right and bottom to top. Since the ray's first hit is used, this may behave
             * unexpectedly with reflective surfaces, transparent surfaces, invisible surfaces, portals etc.
             * The ray has a a time value of 1, for the current state.
             *
             * @param scene Scene to use to find the object to focus on.
             * @param position Image space coordinates on which to focus. [horizontal, vertical] from 0 to 1, from left to right and bottom to top.
             */
            virtual auto autoFocus(const Scene_t* scene, std::array<double, 2> position) -> void final;

            /**
             * @brief Zooms the camera's field of view by a factor.
             *
             * This is used to change the camera's field of view by a factor.
             *
             * @param factor Factor by which to zoom the camera.
             */
            virtual auto zoom(double factor) -> void final;

            /**
             * @brief Writes the image buffer to disk with the provided name.
             *
             * This will write the camera's image to disk. It uses the input name.
             * This calls the image buffer's write function. Directory must exist.
             *
             * @param file_name Filename used to write the images.
             */
            virtual auto write(const std::string& file_name) -> void final;

            /**
             * @brief Writes the image buffer to disk with the camera's filename.
             *
             * This will write the camera's image to disk. It uses the camera's filename_.
             * This calls the image buffer's write function. Directory must exist.
             */
            virtual auto write() -> void final;

            /**
             * @brief Shows the image on screen. Currently not implemented. Use ImgBufferOpenGL_t instead.
             */
            virtual auto show() const -> void final;

            /**
             * @brief Resets the camera's image buffer, for when the scene or camera has changed.
             *
             * This will discard all accumulated samples and start accumulation from scratch. Calls the image buffer's
             * reset function.
             */
            virtual auto reset() -> void final;
    };
}}
#endif