#ifndef APTRACER_IMGBUFFEROPENGL_T_H
#define APTRACER_IMGBUFFEROPENGL_T_H

#include "entities/ImgBuffer_t.h"
#include "entities/Vec3f.h"
#include <string>

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

namespace APTracer { namespace Entities {
    /**
     * @brief The ImgBufferOpenGL_t class represents an image, which can be modified, can accumulate samples, can be written to disk. It is shown on screen in a window.
     * 
     * This class is derived from ImgBuffer_t, adding the capability to be shown on screen and interact with OpenGL.
     */
    class ImgBufferOpenGL_t final : public ImgBuffer_t{
        public:
            /**
             * @brief Construct a new ImgBufferOpenGL_t object with the given dimensions.
             * 
             * @param size_x Horizontal number of pixels in the image.
             * @param size_y Vertical number of pixels in the image.
             */
            ImgBufferOpenGL_t(unsigned int size_x, unsigned int size_y);

            /**
             * @brief Destroy the ImgBufferOpenGL_t object, freeing the memory taken up by the image.
             */
            virtual ~ImgBufferOpenGL_t() final;

            unsigned char* img_gl_; /**< @brief Array of colour pixels representing the image, of length size_x_*size_y_. In integer format, the actual data used by OpenGL to display the image.*/
            GLuint tex_; /**< @brief OpenGL ID of the image. This represents the texture to which the program renders the image.*/

            /**
             * @brief Resets the image, discarding all samples to date.
             * 
             * Sets the number of updates to 0, and sets all pixels to 0, 0, 0.
             */
            virtual void reset() final;

            /**
             * @brief Updates the whole image, adding the contribution of all pixels in the input.
             * 
             * This will increase the number of updates of the image by 1.
             * 
             * @param img Array of colour pixels to be added to the image.
             * @param size_x Horizontal size of the image to be added.
             * @param size_y Vertical size of the image to be added.
             */
            virtual void update(const Vec3f* img, unsigned int size_x, unsigned int size_y) final;

            /**
             * @brief Increments the number of updates of the image by 1.
             * 
             * This increments the number of samples in the image. Useful when updating pixels individually, 
             * which doesn't change the number of updates of the image. It also sends the updated image to OpenGL
             * for displaying.
             */
            virtual void update() final;

            /**
             * @brief Updates a single pixel of the image, adding the contribution of the input.
             * 
             * This doesn't increase the number of updates of the image.
             * 
             * @param colour Colour contribution to be added to the pixel.
             * @param pos_x Horizontal coordinate of the pixel to be updated.
             * @param pos_y Vertical coordinate of the pixel to be updated.
             */
            virtual void update(const Vec3f &colour, unsigned int pos_x, unsigned int pos_y) final;

            /**
             * @brief Sets the value of the whole imagee to the value of all pixels in the input.
             * 
             * This sets the number of updates to 1.
             * 
             * @param img Array of colour pixels to be given to the image.
             * @param size_x Horizontal size of the image to be set.
             * @param size_y Vertical size of the image to be set.
             */
            virtual void set(const Vec3f* img, unsigned int size_x, unsigned int size_y) final;

            /**
             * @brief Writes the image to disk using the provided filename and gamma.
             * 
             * This converts the image to integer values and saves it to a 16bit png file. The image
             * is encoded with the provided gamma value, 1.0 being standard.
             * 
             * @param filename File to which the image will be saved.
             * @param gammaind Gamma used to encode the image. Defaults to 1.0.
             */
            virtual void set(const Vec3f &colour, unsigned int pos_x, unsigned int pos_y) final;
    };
}}
#endif