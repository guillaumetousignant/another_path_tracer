#ifndef APTRACER_CAM3D_T_H
#define APTRACER_CAM3D_T_H

#include "entities/Camera_t.h"
#include "entities/Vec3f.h"
#include <string>
#include <list>
#include <random>
#include <memory>

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
    class Cam_t;
    
    /**
     * @brief The cam 3D class describes a camera that uses a spherical projection and generates 3D images.
     * 
     * It uses a spherical projection [r, theta, phi], where rays are equally spaced 
     * in the theta and phi directions. This reduces warping at higher fields of view,
     * and permits the usage of arbitrarily high fields of view.
     * This camera has no other effects like motion blur or aperture.
     * This camera generates two images from offset positions and combines them to
     * create a 3D anaglyph image.
     */
    class Cam3D_t  final: public Camera_t{
        public:
            /**
             * @brief Construct a new Cam3D_t object. Creates two dependant cameras, one for each eye. Most arguments are passed to them.
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
             * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
             */
            Cam3D_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, std::array<double, 2> fov, std::array<unsigned int, 2> subpix, ImgBuffer_t* image, ImgBuffer_t* image_L, ImgBuffer_t* image_R, double eye_dist, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double focal_length, double gammaind);

            ImgBuffer_t* image_; /**< @brief Image buffer into which the resulting 3D anaglyph image is stored.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            double eye_dist_; /**< @brief Distance between the left and right eye cameras. Higher values will make the 3D effect stronger.*/
            double focal_length_; /**< @brief Distance of the focal plane to the camera origin. Distance at which both eye cameras converge.*/
            double focal_length_buffer_; /**< @brief Focal length to be modified between updates. Its value is given to the real focal length on update.*/
            std::unique_ptr<Cam_t> camera_L_; /**< @brief Left eye camera. Managed by the 3D camera, do not update directly.*/
            std::unique_ptr<Cam_t> camera_R_; /**< @brief Right eye camera. Managed by the 3D camera, do not update directly.*/

            /**
             * @brief Updates the camera's members and the dependant cameras.
             * 
             * This is used to set the new direction, origin, up vector, and focal length. Should be called once per frame, before rendering. 
             * This is how the changes to the transformation matrix and functions like setUp take effect.
             * The left and right eye cameras are also updated directly.
             */
            virtual auto update() -> void final;

            /**
             * @brief Sends rays through the scene, to generate an image.
             * 
             * Both the left and right eye cameras will generate rays according to their projection, and cast them through the provided scene.
             * The resulting colour is written to the image buffer. This will generate one image per camera.
             * Then, the images are combined into the 3D camera's image buffer, to create a 3D anaglyph image.
             * 
             * @param scene Scene that will be used to find what each ray hits.
             */
            virtual auto raytrace(const Scene_t* scene) -> void final;

            /**
             * @brief Writes the image buffer and the left and right eye images to disk with the provided name.
             * 
             * This will write the camera's image to disk. It uses the input name.
             * This calls the image buffer's write function. Directory must exist.
             * The 3D image will use the file name, and the left and right images will have _L and _R suffixes, respectively.
             * 
             * @param file_name Filename used to write the 3D image and the left and right images will have _L and _R suffixes.
             */
            virtual auto write(const std::string& file_name) -> void final;

            /**
             * @brief Writes the image buffer and the left and right eye images to disk with the camera's filename.
             * 
             * This will write the camera's image to disk. It uses the camera's filename_ .
             * This calls the image buffer's write function. Directory must exist.
             * The 3D image will use the file name, and the left and right images will have _L and _R suffixes, respectively.
             */
            virtual auto write() -> void final;

            /**
             * @brief Shows the image on screen. Currently not implemented. Use ImgBufferOpenGL_t instead.
             */
            virtual auto show() const -> void final;
            
            /**
             * @brief Resets the camera's and left and right cameras' image buffers, for when the scene or camera has changed.
             * 
             * This will discard all accumulated samples and start accumulation from scratch. Calls the image buffers'
             * reset functions.
             */
            virtual auto reset() -> void final;

            /**
             * @brief Sets the focal length of the camera, the distance at which both eye cameras converge.
             * 
             * The focal length will be changed on next update along with other members.
             * Distance at which both eye cameras converge, objects away from that distance will be out of focus.
             * 
             * @param focus_distance New focal length of the camera. Distance at which both eye cameras converge.
             */
            virtual auto focus(double focus_distance) -> void final;
            
            /**
             * @brief Focusses the camera on a point in its field of view.
             * 
             * This will send a ray at the specified coordinates in image space and set the focal length as
             * the length to its first intersection. The coordinates are [horizontal, vertical] from 0 to 1,
             * from left to right and bottom to top. Since the ray's first hit is used, this may behave 
             * unexpectedly with reflective surfaces, transparent surfaces, invisible surfaces, portals etc.
             * The ray has its origin between the two eye cameras.
             * 
             * @param scene Scene to use to find the object to focus on.
             * @param position Image space coordinates on which to focus. [horizontal, vertical] from 0 to 1, from left to right and bottom to top.
             */
            virtual auto autoFocus(const Scene_t* scene, std::array<double, 2> position) -> void final;
    };
}}
#endif