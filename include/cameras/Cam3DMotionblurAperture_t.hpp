#ifndef APTRACER_CAMERAS_CAM3DMOTIONBLURAPERTURE_T_HPP
#define APTRACER_CAMERAS_CAM3DMOTIONBLURAPERTURE_T_HPP

#include "cameras/CamMotionblurAperture_t.hpp"
#include "entities/Camera_t.hpp"
#include "entities/ImgBuffer_t.hpp"
#include <list>
#include <memory>
#include <random>
#include <string>

namespace APTracer { namespace Cameras {

    /**
     * @brief The cam 3D motionblur aperture class describes a camera that uses a spherical projection, a shutter for motion blur, has an aperture for depth of field and generates 3D images
     *
     * It uses a spherical projection [r, theta, phi], where rays are equally spaced
     * in the theta and phi directions. This reduces warping at higher fields of view,
     * and permits the usage of arbitrarily high fields of view.
     * This camera creates rays with a random time between the last state and current
     * state of the scene. This enables motion blur, as objects can use this to interpolate
     * their state at the ray's time. The rays are also created with the interpolated
     * camera state.
     * This camera creates rays on a disk to simulate the aperture of a real camera for depth of field.
     * It has a focus distance, so all objects at that distance will be in the focal plane,
     * and other objects will be out of focus. The shape of the focal plane is a sphere
     * of focus_distance_ radius centered on the origin.
     * This camera generates two images from offset positions and combines them to
     * create a 3D anaglyph image.
     */
    class Cam3DMotionblurAperture_t final : public Entities::Camera_t {
        public:
            /**
             * @brief Construct a new Cam3DMotionblurAperture_t object. Creates two dependant cameras, one for each eye. Most arguments are passed to them.
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
             * @param focus_distance Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus. Distance at which both eye cameras converge.
             * @param aperture Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.
             * @param time Opening and closing time of the shutter. [open, close], from 0 to 1, where 0 is last state and current state. Rays are created at a time in this interval. Enables motion
             * blur.
             * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
             */
            Cam3DMotionblurAperture_t(Entities::TransformMatrix_t* transformation,
                                      const std::string& filename,
                                      Entities::Vec3f up,
                                      std::array<double, 2> fov,
                                      std::array<unsigned int, 2> subpix,
                                      Entities::ImgBuffer_t* image,
                                      Entities::ImgBuffer_t* image_L,
                                      Entities::ImgBuffer_t* image_R,
                                      double eye_dist,
                                      std::list<Entities::Medium_t*> medium_list,
                                      Entities::Skybox_t* skybox,
                                      unsigned int max_bounces,
                                      double focus_distance,
                                      double aperture,
                                      std::array<double, 2> time,
                                      double gammaind);

            /**
             * @brief Destroy the Cam3DMotionblurAperture_t object.
             */
            ~Cam3DMotionblurAperture_t() final = default;

            /**
             * @brief Copy construct a new Cam3DMotionblurAperture_t object.
             *
             * @param other Object to copy.
             */
            Cam3DMotionblurAperture_t(const Cam3DMotionblurAperture_t& other);

            /**
             * @brief Move construct a new Cam3DMotionblurAperture_t object.
             *
             * @param other Object to move.
             */
            Cam3DMotionblurAperture_t(Cam3DMotionblurAperture_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return Cam3DMotionblurAperture_t& Reference to this object.
             */
            auto operator=(const Cam3DMotionblurAperture_t& other) -> Cam3DMotionblurAperture_t&;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return Cam3DMotionblurAperture_t& Reference to this object.
             */
            auto operator=(Cam3DMotionblurAperture_t&& other) noexcept -> Cam3DMotionblurAperture_t& = default;

            Entities::ImgBuffer_t* image_; /**< @brief Image buffer into which the resulting 3D anaglyph image is stored.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            double eye_dist_; /**< @brief Distance between the left and right eye cameras. Higher values will make the 3D effect stronger.*/
            double focus_distance_; /**< @brief Distance of the focal plane to the camera origin. Objects away from that distance will be out of focus. The focal plane has the shape of a sphere with
                                       this radius. Distance at which both eye cameras converge.*/
            double focus_distance_buffer_; /**< @brief Focus distance to be modified between updates. Its value is given to the real focus distance on update.*/
            double aperture_; /**< @brief Radius of the disk on which rays are created around the origin. Higher values will cause stronger depth of field, objects out of focus will be blurrier.*/

            Entities::Vec3f direction_last_; /**< @brief Direction in which the camera points before last update. Used for motion blur.*/
            Entities::Vec3f origin_last_; /**< @brief Position of the camera before last update. Used for motion blur.*/
            double focus_distance_last_; /**< @brief Distance of the focal plane to the camera origin before last update. Used for motion blur.*/
            std::array<double, 2> time_; /**< @brief Opening and closing time of the shutter. [open, close], from 0 to 1, where 0 is last state and current state. Rays are created at a time in this
                                            interval. Enables motion blur.*/
            Entities::Vec3f up_last_; /**< @brief Vector pointing up before last update. Used for motion blur.*/
            std::array<double, 2> fov_last_; /**< @brief Field of view before last update. Used for motion blur.*/

            std::unique_ptr<CamMotionblurAperture_t> camera_L_; /**< @brief Left eye camera. Managed by the 3D camera, do not update directly.*/
            std::unique_ptr<CamMotionblurAperture_t> camera_R_; /**< @brief Right eye camera. Managed by the 3D camera, do not update directly.*/
            std::array<double, 2> focus_coordinates_; /**< @brief Image-space coordinates on which the camera is focussed. [horizontal, vertical] from 0 to 1, from left to right and bottom to top.*/

            /**
             * @brief Updates the camera's members and the dependant cameras.
             *
             * This is used to set the new direction, origin, up vector, and focus distance. Should be called once per frame, before rendering.
             * This is how the changes to the transformation matrix and functions like setUp take effect. It sets focus_distance_ to focus_distance_buffer_,
             * so it is only modified once per frame. This is needed to correctly interpolate focus distance for motion blur.
             * Stores the previous state in the _last variables, so that state can be interpolated according to time.
             * The left and right eye cameras are also updated directly.
             */
            auto update() -> void final;

            /**
             * @brief Sends rays through the scene, to generate an image.
             *
             * Both the left and right eye cameras will generate rays according to their projection, and cast them through the provided scene.
             * The resulting colour is written to the image buffer. This will generate one image per camera.
             * Then, the images are combined into the 3D camera's image buffer, to create a 3D anaglyph image.
             * Rays are created with a time inside the time_ interval, to simulate a shutter and enable motion blur.
             * Rays are created on a disk to simulate an aperture and depth of field.
             *
             * @param scene Scene that will be used to find what each ray hits.
             */
            auto raytrace(const Entities::Scene_t* scene) -> void final;

            /**
             * @brief Zooms the camera's field of view by a factor.
             *
             * This is used to change the camera's field of view by a factor.
             *
             * @param factor Factor by which to zoom the camera.
             */
            auto zoom(double factor) -> void final;

            /**
             * @brief Writes the image buffer and the left and right eye images to disk with the provided name.
             *
             * This will write the camera's image to disk. It uses the input name.
             * This calls the image buffer's write function. Directory must exist.
             * The 3D image will use the file name, and the left and right images will have _L and _R suffixes, respectively.
             *
             * @param file_name Filename used to write the 3D image and the left and right images will have _L and _R suffixes.
             */
            auto write(const std::string& file_name) -> void final;

            /**
             * @brief Writes the image buffer and the left and right eye images to disk with the camera's filename.
             *
             * This will write the camera's image to disk. It uses the camera's filename_ .
             * This calls the image buffer's write function. Directory must exist.
             * The 3D image will use the file name, and the left and right images will have _L and _R suffixes, respectively.
             */
            auto write() -> void final;

            /**
             * @brief Shows the image on screen. Currently not implemented. Use ImgBufferOpenGL_t instead.
             */
            auto show() const -> void final;

            /**
             * @brief Resets the camera's and left and right cameras' image buffers, for when the scene or camera has changed.
             *
             * This will discard all accumulated samples and start accumulation from scratch. Calls the image buffers'
             * reset functions.
             */
            auto reset() -> void final;

            /**
             * @brief Sets the focus distance of the camera, the distance at which both eye cameras converge.
             *
             * The focus distance will be changed on next update along with other members.
             * All objects at that distance will be in the focal plane, and other objects will be out of focus.
             * The focal plane has the shape of a sphere with this radius. Distance at which both eye cameras converge.
             *
             * @param focus_distance New focus distance of the camera. Distance at which both eye cameras converge.
             */
            auto focus(double focus_distance) -> void final;

            /**
             * @brief Focusses the camera on a point in its field of view.
             *
             * This will send a ray at the specified coordinates in image space and set the focus distance as
             * the length to its first intersection. The coordinates are [horizontal, vertical] from 0 to 1,
             * from left to right and bottom to top. Since the ray's first hit is used, this may behave
             * unexpectedly with reflective surfaces, transparent surfaces, invisible surfaces, portals etc.
             * The ray has a a time value of 1, for the current state.
             * The ray has its origin between the two eye cameras.
             *
             * @param scene Scene to use to find the object to focus on.
             * @param position Image space coordinates on which to focus. [horizontal, vertical] from 0 to 1, from left to right and bottom to top.
             */
            auto autoFocus(const Entities::Scene_t* scene, std::array<double, 2> position) -> void final;
    };
}}
#endif