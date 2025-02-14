#ifndef APTRACER_ENTITIES_IMGBUFFER_T_HPP
#define APTRACER_ENTITIES_IMGBUFFER_T_HPP

#include "entities/Vec3f.hpp"
#include <string>
#include <vector>

namespace APTracer { namespace Entities {
    /**
     * @brief The ImgBuffer_t class represents an image, which can be modified, can accumulate samples, and can be written to disk.
     *
     * This is the class that stores the results of the intersections. Generally, cameras shoot rays through a scene, and afterwards
     * add that ray's contribution to the image buffer.
     */
    class ImgBuffer_t {
        public:
            /**
             * @brief Construct a new ImgBuffer_t object with the given dimensions.
             *
             * @param size_x Horizontal number of pixels in the image.
             * @param size_y Vertical number of pixels in the image.
             */
            ImgBuffer_t(size_t size_x, size_t size_y);

            /**
             * @brief Destroy the ImgBuffer_t object.
             */
            virtual ~ImgBuffer_t() = default;

            /**
             * @brief Copy construct a new ImgBuffer_t object.
             *
             * @param other Object to copy.
             */
            ImgBuffer_t(const ImgBuffer_t& other) = default;

            /**
             * @brief Move construct a new ImgBuffer_t object.
             *
             * @param other Object to move.
             */
            ImgBuffer_t(ImgBuffer_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return ImgBuffer_t& Reference to this object.
             */
            auto operator=(const ImgBuffer_t& other) -> ImgBuffer_t& = default;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return ImgBuffer_t& Reference to this object.
             */
            auto operator=(ImgBuffer_t&& other) noexcept -> ImgBuffer_t& = default;

            size_t size_x_; /**< @brief Horizontal number of pixels in the image. Main axis of the layout.*/
            size_t size_y_; /**< @brief Vertical number of pixels in the image. Secondary axis of the layout.*/
            unsigned int updates_; /**< @brief Number of times the whole image was updated. Number of samples that each pixel holds.*/
            std::vector<Vec3f> img_; /**< @brief Array of colour pixels representing the image, of length size_x_*size_y_. Stored in a x-major fashion, accessed with: pos_y*size_x_ + pos_x.*/

            /**
             * @brief Resets the image, discarding all samples to date.
             *
             * Sets the number of updates to 0, and sets all pixels to 0, 0, 0.
             */
            virtual auto reset() -> void;

            /**
             * @brief Updates the whole image, adding the contribution of all pixels in the input.
             *
             * This will increase the number of updates of the image by 1.
             *
             * @param img Vector of colour pixels to be added to the image.
             */
            virtual auto update(const std::vector<Vec3f>& img) -> void;

            /**
             * @brief Increments the number of updates of the image by 1.
             *
             * This increments the number of samples in the image. Useful when updating pixels individually,
             * which doesn't change the number of updates of the image.
             */
            virtual auto update() -> void;

            /**
             * @brief Updates a single pixel of the image, adding the contribution of the input.
             *
             * This doesn't increase the number of updates of the image.
             *
             * @param colour Colour contribution to be added to the pixel.
             * @param pos_x Horizontal coordinate of the pixel to be updated.
             * @param pos_y Vertical coordinate of the pixel to be updated.
             */
            virtual auto update(const Vec3f& colour, size_t pos_x, size_t pos_y) -> void;

            /**
             * @brief Sets the value of the whole image to the value of all pixels in the input.
             *
             * This sets the number of updates to 1.
             *
             * @param img Vector of colour pixels to be given to the image.
             */
            virtual auto set(const std::vector<Vec3f>& img) -> void;

            /**
             * @brief Sets the value of a single pixel of the image to the value of the input.
             *
             * This doesn't change the number of updates of the image.
             *
             * @param colour Colour contribution to be given to the pixel.
             * @param pos_x Horizontal coordinate of the pixel to be set.
             * @param pos_y Vertical coordinate of the pixel to be set.
             */
            virtual auto set(const Vec3f& colour, size_t pos_x, size_t pos_y) -> void;

            /**
             * @brief Writes the image to disk using the provided filename.
             *
             * This converts the image to integer values and saves it to a 16bit png file. The image
             * is encoded with a gamma of 1.0.
             *
             * @param filename File to which the image will be saved.
             */
            auto write(const std::string& filename) const -> void;

            /**
             * @brief Writes the image to disk using the provided filename and gamma.
             *
             * This converts the image to integer values and saves it to a 16bit png file. The image
             * is encoded with the provided gamma value, 1.0 being standard.
             *
             * @param filename File to which the image will be saved.
             * @param gammaind Gamma used to encode the image.
             */
            auto write(const std::string& filename, double gammaind) const -> void;
    };
}}
#endif
