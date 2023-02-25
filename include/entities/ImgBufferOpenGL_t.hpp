#ifndef APTRACER_ENTITIES_IMGBUFFEROPENGL_T_HPP
#define APTRACER_ENTITIES_IMGBUFFEROPENGL_T_HPP

#include "entities/ImgBuffer_t.hpp"
#include "entities/Vec3f.hpp"
#include <string>

#ifdef APTRACER_USE_OPENGL
    #ifdef _WIN32
        #include "GL/freeglut.h"
    #else
        #ifndef __APPLE__
            #include "GL/glut.h"
        #else
            #include <GLUT/glut.h>
        #endif
    #endif

    #ifndef __APPLE__
        #include "GL/gl.h"
    #endif
#else
typedef unsigned int GLuint;
#endif

namespace APTracer { namespace Entities {
        /**
         * @brief The ImgBufferOpenGL_t class represents an image, which can be modified, can accumulate samples, can be written to disk. It is shown on screen in a window.
         *
         * This class is derived from ImgBuffer_t, adding the capability to be shown on screen and interact with OpenGL.
         */
        class ImgBufferOpenGL_t final : public ImgBuffer_t {
            public:
                /**
                 * @brief Construct a new ImgBufferOpenGL_t object with the given dimensions.
                 *
                 * @param size_x Horizontal number of pixels in the image.
                 * @param size_y Vertical number of pixels in the image.
                 */
                ImgBufferOpenGL_t(size_t size_x, size_t size_y);

                std::vector<unsigned char>
                    img_gl_; /**< @brief Array of colour pixels representing the image, of length size_x_*size_y_. In integer format, the actual data used by OpenGL to display the image.*/
                GLuint tex_; /**< @brief OpenGL ID of the image. This represents the texture to which the program renders the image.*/

                /**
                 * @brief Resets the image, discarding all samples to date.
                 *
                 * Sets the number of updates to 0, and sets all pixels to 0, 0, 0.
                 */
                auto reset() -> void final;

                /**
                 * @brief Updates the whole image, adding the contribution of all pixels in the input.
                 *
                 * This will increase the number of updates of the image by 1.
                 *
                 * @param img Vector of colour pixels to be added to the image.
                 */
                auto update(const std::vector<Vec3f>& img) -> void final;

                /**
                 * @brief Increments the number of updates of the image by 1.
                 *
                 * This increments the number of samples in the image. Useful when updating pixels individually,
                 * which doesn't change the number of updates of the image. It also sends the updated image to OpenGL
                 * for displaying.
                 */
                auto update() -> void final;

                /**
                 * @brief Updates a single pixel of the image, adding the contribution of the input.
                 *
                 * This doesn't increase the number of updates of the image.
                 *
                 * @param colour Colour contribution to be added to the pixel.
                 * @param pos_x Horizontal coordinate of the pixel to be updated.
                 * @param pos_y Vertical coordinate of the pixel to be updated.
                 */
                auto update(const Vec3f& colour, size_t pos_x, size_t pos_y) -> void final;

                /**
                 * @brief Sets the value of the whole image to the value of all pixels in the input.
                 *
                 * This sets the number of updates to 1.
                 *
                 * @param img Vector of colour pixels to be given to the image.
                 */
                auto set(const std::vector<Vec3f>& img) -> void final;

                /**
                 * @brief Sets the value of a single pixel of the image to the input value.
                 *
                 * This doesn't reset the number of updates to 1.
                 *
                 * @param colour Colour to be given to the pixel.
                 * @param pos_x Horizontal coordinate of the pixel to be set.
                 * @param pos_y Vertical coordinate of the pixel to be set.
                 */
                auto set(const Vec3f& colour, size_t pos_x, size_t pos_y) -> void final;
        };
}}
#endif