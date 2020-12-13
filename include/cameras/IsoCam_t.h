#ifndef APTRACER_ISOCAM_T_H
#define APTRACER_ISOCAM_T_H

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
     * @brief The iso cam class describes a camera that uses an isometric projection.
     * 
     * It uses an isometric projection, where rays have the same direction and are equally spaced 
     * on a plane for their origin. This removes perspective from the image. As such, its field
     * of view is not an angle, but a width and a height.
     * This camera stores the result from its rays in a single image buffer, and has no
     * other effects like motion blur or aperture.
     */
    class IsoCam_t  final: public Camera_t{
        public:
            /**
             * @brief Construct a new IsoCam_t object. Most arguments are passed to the Camera_t constructor.
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
             * @param gammaind Gamma of the saved picture. A value of 1 should be used for usual cases.
             */
            IsoCam_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind);
            
            /**
             * @brief Destroy the IsoCam_t object. Does nothing.
             */
            virtual ~IsoCam_t() final;

            ImgBuffer_t* image_; /**< @brief Image buffer into which the image is stored.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Updates the camera's members.
             * 
             * This is used to set the new direction, origin and up vector. Should be called once per frame, before rendering. 
             * This is how the changes to the transformation matrix and functions like setUp take effect.
             */
            virtual void update() final;

            /**
             * @brief Sends rays through the scene, to generate an image.
             * 
             * The camera will generate rays according to an isometric projection, and cast them through the provided scene. 
             * The resulting colour is written to the image buffer. This will generate one image.
             * 
             * @param scene Scene that will be used to find what each ray hits.
             */
            virtual void raytrace(const Scene_t* scene) final;

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