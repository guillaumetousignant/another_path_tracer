#ifndef APTRACER_CAMERAS_CAMAPERTURE_T_HPP
#define APTRACER_CAMERAS_CAMAPERTURE_T_HPP

#include "entities/Camera_t.hpp"
#include "entities/ImgBuffer_t.hpp"
#include <list>
#include <random>
#include <string>

namespace APTracer { namespace Cameras {

    /**
     * @brief The cam aperture class describes a camera that uses a spherical projection and has an aperture, for depth of field.
     *
     * It uses a spherical projection [r, theta, phi], where rays are equally spaced
     * in the theta and phi directions. This reduces warping at higher fields of view,
     * and permits the usage of arbitrarily high fields of view.
     * This camera creates rays on a disk to simulate the aperture of a real camera for depth of field.
     * It has a focus distance, so all objects at that distance will be in the focal plane,
     * and other objects will be out of focus. The shape of the focal plane is a sphere
     * of focus_distance_ radius centered on the origin.
     * This camera stores the result from its rays in a single image buffer, and has no
     * motion blur.
     */
    class CamAperture_t final : public Entities::Camera_t {
        public:
            /**
             * @brief Construct a new CamAperture_t object. Most arguments are passed to the Camera_t constructor.
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
             * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
             */
            CamAperture_t(Entities::TransformMatrix_t* transformation,
                          const std::string& filename,
                          Entities::Vec3f up,
                          std::array<double, 2> fov,
                          std::array<unsigned int, 2> subpix,
                          Entities::ImgBuffer_t* image,
                          std::list<Entities::Medium_t*> medium_list,
                          Entities::Skybox_t* skybox,
                          unsigned int max_bounces,
                          double focus_distance,
                          double aperture,
                          double gammaind);

            Entities::ImgBuffer_t* image_; /**< @brief Image buffer into which the image is stored.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            double focus_distance_; /**< @brief Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus. The focal plane has the shape of a sphere with
                                       this radius.*/
            double aperture_; /**< @brief Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.*/
            double focus_distance_buffer_; /**< @brief Focus distance to be modified between updates. Its value is given to the real focus distance on update.*/

            /**
             * @brief Updates the camera's members.
             *
             * This is used to set the new direction, origin, up vector, and focus distance. Should be called once per frame, before rendering.
             * This is how the changes to the transformation matrix and functions like setUp take effect. It sets focus_distance_ to focus_distance_buffer_,
             * so it is only modified once per frame. This is needed for some features to work, like motion blur.
             */
            auto update() -> void final;

            /**
             * @brief Sends rays through the scene, to generate an image.
             *
             * The camera will generate rays according to a spherical projection, and cast them through the provided scene.
             * The resulting colour is written to the image buffer. This will generate one image. Rays are created
             * on a disk to simulate an aperture and depth of field.
             *
             * @param scene Scene that will be used to find what each ray hits.
             */
            auto raytrace(const Entities::Scene_t* scene) -> void final;

            /**
             * @brief Sets the focus distance of the camera.
             *
             * The focus distance will be changed on next update along with other members.
             * All objects at that distance will be in the focal plane, and other objects will be out of focus.
             * The focal plane has the shape of a sphere with this radius.
             *
             * @param focus_distance New focus distance of the camera.
             */
            auto focus(double focus_distance) -> void final;

            /**
             * @brief Focusses the camera on a point in its field of view.
             *
             * This will send a ray at the specified coordinates in image space and set the focus distance as
             * the length to its first intersection. The coordinates are [horizontal, vertical] from 0 to 1,
             * from left to right and bottom to top. Since the ray's first hit is used, this may behave
             * unexpectedly with reflective surfaces, transparent surfaces, invisible surfaces, portals etc.
             *
             * @param scene Scene to use to find the object to focus on.
             * @param position Image space coordinates on which to focus. [horizontal, vertical] from 0 to 1, from left to right and bottom to top.
             */
            auto autoFocus(const Entities::Scene_t* scene, std::array<double, 2> position) -> void final;

            /**
             * @brief Zooms the camera's field of view by a factor.
             *
             * This is used to change the camera's field of view by a factor.
             *
             * @param factor Factor by which to zoom the camera.
             */
            auto zoom(double factor) -> void final;

            /**
             * @brief Writes the image buffer to disk with the provided name.
             *
             * This will write the camera's image to disk. It uses the input name.
             * This calls the image buffer's write function. Directory must exist.
             *
             * @param file_name Filename used to write the images.
             */
            auto write(const std::string& file_name) -> void final;

            /**
             * @brief Writes the image buffer to disk with the camera's filename.
             *
             * This will write the camera's image to disk. It uses the camera's filename_.
             * This calls the image buffer's write function. Directory must exist.
             */
            auto write() -> void final;

            /**
             * @brief Shows the image on screen. Currently not implemented. Use ImgBufferOpenGL_t instead.
             */
            auto show() const -> void final;

            /**
             * @brief Resets the camera's image buffer, for when the scene or camera has changed.
             *
             * This will discard all accumulated samples and start accumulation from scratch. Calls the image buffer's
             * reset function.
             */
            auto reset() -> void final;
    };
}}
#endif
