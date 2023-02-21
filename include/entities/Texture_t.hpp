#ifndef APTRACER_ENTITIES_TEXTURE_T_HPP
#define APTRACER_ENTITIES_TEXTURE_T_HPP

#include "entities/Vec3f.hpp"
#include <array>
#include <string>
#include <vector>

namespace APTracer { namespace Entities {

    /**
     * @brief The texture class represents an image or other 2D buffer that can be accessed using 2D coordinates.
     *
     * This is used to store image or other 2D maps, such as normal maps or emissivity maps. They can be accessed
     * in 2D coordinates, for example using texture coordinates from meshes.
     */
    class Texture_t {
        public:
            /**
             * @brief Construct a new Texture_t object from an image file.
             *
             * @param filename Path to the image to be stored in the texture.
             */
            Texture_t(const std::string& filename);

            size_t size_x_; /**< @brief Horizontal number of pixels in the texture*/
            size_t size_y_; /**< @brief Vertical number of pixels in the texture*/
            std::vector<Vec3f> img_; /**< @brief Array of 3-element pixels representing the values of all pixels of the texture.*/

            /**
             * @brief Returns the value of the texture from 2D uniform coordinates, using bicubic filtering.
             *
             * This returns the blended value of the sixteen pixels around the given point, according to their proximity.
             * It uses a Catmull-Rom Hermite spline.
             * From demofox on Shadertoy, https://www.shadertoy.com/view/MllSzX
             *
             * @param xy Array of two coordinates, [x, y], ranging from 0 to 1, starting from the bottom left of the texture.
             * @return Vec3f Value of the texture at the provided coordinates.
             */
            auto get(std::array<double, 2> xy) const -> Vec3f;

            /**
             * @brief Returns the value of the texture from 2D uniform coordinates, using bicubic filtering.
             *
             * This returns the blended value of the sixteen pixels around the given point, according to their proximity.
             * It uses a Catmull-Rom Hermite spline.
             * From demofox on Shadertoy, https://www.shadertoy.com/view/MllSzX
             *
             * @param xy Array of two coordinates, [x, y], ranging from 0 to 1, starting from the bottom left of the texture.
             * @return Vec3f Value of the texture at the provided coordinates.
             */
            auto get_cubic(std::array<double, 2> xy) const -> Vec3f;

            /**
             * @brief Returns the value of the texture from 2D uniform coordinates, using bilinear filtering.
             *
             * This returns the blended value of the four pixels around the given point, according to their proximity.
             *
             * @param xy Array of two coordinates, [x, y], ranging from 0 to 1, starting from the bottom left of the texture.
             * @return Vec3f Value of the texture at the provided coordinates.
             */
            auto get_linear(std::array<double, 2> xy) const -> Vec3f;

            /**
             * @brief Returns the value of the texture from 2D uniform coordinates, using nearest-neighbour filtering.
             *
             * This returns the value of the closest pixel to the given point.
             *
             * @param xy Array of two coordinates, [x, y], ranging from 0 to 1, starting from the bottom left of the texture.
             * @return Vec3f Value of the texture at the provided coordinates.
             */
            auto get_nn(std::array<double, 2> xy) const -> Vec3f;

        private:
            /**
             * @brief Returns the cubic Hermite spline of four pixels and a distance t.
             *
             * This is the Catmull-Rom spline.
             * From demofox on Shadertoy, https://www.shadertoy.com/view/MllSzX
             *
             * @param A First pixel.
             * @param B Second pixel.
             * @param C Third pixel.
             * @param D Fourth pixel.
             * @param t Distance from the center.
             * @return Vec3f Resulting pixel.
             */
            static auto CubicHermite(Vec3f A, Vec3f B, Vec3f C, Vec3f D, double t) -> Vec3f;
    };
}}

#endif