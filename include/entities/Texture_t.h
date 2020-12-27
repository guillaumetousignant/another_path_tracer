#ifndef APTRACER_TEXTURE_T_H
#define APTRACER_TEXTURE_T_H

#include "entities/Vec3f.h"
#include <string>
#include <array>

namespace APTracer { namespace Entities {

    /**
     * @brief The texture class represents an image or other 2D buffer that can be accessed using 2D coordinates.
     * 
     * This is used to store image or other 2D maps, such as normal maps or emissivity maps. They can be accessed
     * in 2D coordinates, for example using texture coordinates from meshes.
     */
    class Texture_t{
        public:
            /**
             * @brief Construct a new Texture_t object from an image file.
             * 
             * @param filename Path to the image to be stored in the texture.
             */
            Texture_t(const std::string &filename);

            /**
             * @brief Destroy the Texture_t object, releasing the ressources held by it.
             */
            ~Texture_t();

            unsigned int size_x_; /**< @brief Horizontal number of pixels in the texture*/
            unsigned int size_y_; /**< @brief Vertical number of pixels in the texture*/
            Vec3f* img_; /**< @brief Array of 3-element pixels representing the values of all pixels of the texture.*/

            /**
             * @brief Returns the value of the texture from 2D uniform coordinates, using bilinear filtering.
             * 
             * This returns the blended value of the four pixels around the given point, according to their proximity.
             * 
             * @param xy Array of two coordinates, [x, y], ranging from 0 to 1, starting from the bottom left of the texture.
             * @return Vec3f Value of the texture at the provided coordinates.
             */
            Vec3f get(std::array<double, 2> xy) const;

            /**
             * @brief Returns the value of the texture from 2D uniform coordinates, using nearest-neighbour filtering.
             * 
             * This returns the value of the closest pixel to the given point.
             * 
             * @param xy Array of two coordinates, [x, y], ranging from 0 to 1, starting from the bottom left of the texture.
             * @return Vec3f Value of the texture at the provided coordinates.
             */
            Vec3f get_nn(std::array<double, 2> xy) const;
    };
}}

#endif