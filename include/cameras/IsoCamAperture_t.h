#ifndef APTRACER_CAMERAS_ISOCAMAPERTURE_T_H
#define APTRACER_CAMERAS_ISOCAMAPERTURE_T_H

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

using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::ImgBuffer_t;
using APTracer::Entities::Camera_t;
using APTracer::Entities::Vec3f;

namespace APTracer { namespace Cameras {

    /**
     * @brief The iso cam aperture class describes a camera that uses an isometric projection and has an aperture, for depth of field.
     * 
     * It uses an isometric projection, where rays have the same direction and are equally spaced 
     * on a plane for their origin. This removes perspective from the image. As such, its field
     * of view is not an angle, but a width and a height.
     * This camera creates rays on a disk to simulate the aperture of a real camera for depth of field.
     * It has a focal length, so all objects at that distance will be in the focal plane,
     * and other objects will be out of focus. The shape of the focal plane is a plane
     * at focal_length_ from the origin.
     * This camera stores the result from its rays in a single image buffer, and has no
     * motion blur.
     */
    class IsoCamAperture_t final : public Camera_t {
        public:
            /**
             * @brief Construct a new IsoCamAperture_t object. Most arguments are passed to the Camera_t constructor.
             * 
             * @param transformation Transformation matrix used by the camera. Sets the camera's origin and direction when created and updated.
             * @param filename Filename used by the camera when saving an image.
             * @param up Vector pointing up. Used to set the roll of the camera.
             * @param fov Array containing field of view of camera in meters [vertical, horizontal].
             * @param subpix Array containing the number of subpixels per pixel, [vertical, horizontal], for antialiasing purposes. Usually [1, 1].
             * @param image Image buffer into which the resulting image will be stored.
             * @param medium_list Initial list of materials in which the camera is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             * @param skybox Skybox that will be intersected if no shape is hit by the rays.
             * @param max_bounces Maximum intersections with shapes and bounces on materials a ray can do before it is terminated. Actual number may be less.
             * @param focal_length Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus.
             * @param aperture Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.
             * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
             */
            IsoCamAperture_t(TransformMatrix_t* transformation, const std::string &filename, APTracer::Entities::Vec3f up, std::array<double, 2> fov, std::array<unsigned int, 2> subpix, ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double aperture, double gammaind);
            
            ImgBuffer_t* image_; /**< @brief Image buffer into which the image is stored.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            double focal_length_; /**< @brief Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus. The focal plane has the shape of a plane at this distance from the camera.*/
            double aperture_; /**< @brief Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.*/
            double focal_length_buffer_; /**< @brief Focal length to be modified between updates. Its value is given to the real focal length on update.*/

            /**
             * @brief Updates the camera's members.
             * 
             * This is used to set the new direction, origin, up vector, and focal length. Should be called once per frame, before rendering. 
             * This is how the changes to the transformation matrix and functions like setUp take effect. It sets focal_length_ to focal_length_buffer_,
             * so it is only modified once per frame. This is needed for some features to work, like motion blur.
             */
            virtual auto update() -> void final;

            /**
             * @brief Sends rays through the scene, to generate an image.
             * 
             * The camera will generate rays according to an isometric projection, and cast them through the provided scene. 
             * The resulting colour is written to the image buffer. This will generate one image. Rays are created
             * on a disk to simulate an aperture and depth of field.
             * 
             * @param scene 
             */
            virtual auto raytrace(const Scene_t* scene) -> void final;

            /**
             * @brief Sets the focal length of the camera.
             * 
             * The focal length will be changed on next update along with other members.
             * All objects at that distance will be in the focal plane, and other objects will be out of focus.
             * The focal plane has the shape of a plane at this distance from the camera.
             * 
             * @param focus_distance New focal length of the camera.
             */
            virtual auto focus(double focus_distance) -> void final;

            /**
             * @brief Focusses the camera on a point in its field of view.
             * 
             * This will send a ray at the specified coordinates in image space and set the focal length as
             * the length to its first intersection. The coordinates are [horizontal, vertical] from 0 to 1,
             * from left to right and bottom to top. Since the ray's first hit is used, this may behave 
             * unexpectedly with reflective surfaces, transparent surfaces, invisible surfaces, portals etc.
             * 
             * @param scene Scene to use to find the object to focus on.
             * @param position Image space coordinates on which to focus. [horizontal, vertical] from 0 to 1, from left to right and bottom to top.
             */
            virtual auto autoFocus(const Scene_t* scene, std::array<double, 2> position) -> void final;

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