#ifndef APTRACER_MATERIALS_SCATTEREREXPFULL_T_HPP
#define APTRACER_MATERIALS_SCATTEREREXPFULL_T_HPP

#include "entities/Medium_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Vec3f.hpp"
#include <random>

namespace APTracer { namespace Materials {

    /**
     * @brief The scatterer exp full class describes a medium that can absorb and emit light as a ray goes through it, and that can scatter it according to a probability distribution, absorbing and
     * emitting light on scattering.
     *
     * Light is absorbed and/or emitted logarithmically with regards to the distance travelled within it by a ray,
     * according to the Beer-Lambert law.
     * The ray has a chance to be scattered relative to the distance travelled, where it acts ash though it bounced
     * on a particle in the medium. Its origin becomes that intersection point, and is is reflected in a random
     * direction within a portion of the sphere around that intersection point according to a probability distribution.
     * On being scattered, light can be absorbed and emitted.
     * This is useful to cause more back or forward-scattering, to improve convergence for very dense and very light
     * media, respectively.
     * This medium can be used to represent dense interacting media containing particles, dusty atmospheric scattering,
     * sandy water, and very small particles in a dense liquid or solid.
     */
    class ScattererExpFull_t final : public Entities::Medium_t {
        public:
            /**
             * @brief Construct a new ScattererExpFull_t object with a volumetric colour and emissive colour, a scattering colour and emissive colour, the volumetric absorption/emission distance, a
             * scattering distance, the order of the curve describing the distribution of the scattering angle, and a scattering angle.
             *
             * The first two distances are the distances traveled by a ray in the medium to produce the input emissive and absorptive colour, respectively.
             * The scattering distance is the average distance at which a ray will be scattered.
             *
             * @param emi_vol Colour emitted by the medium when a ray traverses abs_dist_emi within it.
             * @param col_vol Colour of the medium when a ray traverses abs_dist_col within it.
             * @param emi_scat Colour emitted by the medium when a ray is scattered.
             * @param col_scat Colour reflected by the medium when a ray is scattered.
             * @param abs_dist_emi Distance travelled through the medium for emi_vol to be emitted.
             * @param abs_dist_col Distance travelled through the medium for col_vol to be absorbed.
             * @param scat_dist Average distance travelled through the medium at which rays are scattered.
             * @param order Order of the curve describing the distribution of the scattering angle, from 0 to inf. 0 is forced to the scattering angle. Between 0 and 1 is back-scatter, 1 is flat, 1 to
             * inf is forward scatter, 2 is parabole, and inf is forward, not changing the direction.
             * @param scattering_angle Portion of the sphere around the intersection point in which the ray will be reflected, from 0 to 1. 0 is forward, not changing the direction, 1 is all the
             * sphere.
             * @param ind Refractive index of the medium.
             * @param priority Priority of the medium over other mediums, used to determine which is the current medium when overlapping. Higher value means higher priority.
             */
            ScattererExpFull_t(Entities::Vec3f emi_vol,
                               Entities::Vec3f col_vol,
                               Entities::Vec3f emi_scat,
                               Entities::Vec3f col_scat,
                               double abs_dist_emi,
                               double abs_dist_col,
                               double scat_dist,
                               double order,
                               double scattering_angle,
                               double ind,
                               unsigned int priority);

            Entities::Vec3f emission_vol_; /**< @brief Volumetric colour emitted by the medium in exponential form.*/
            Entities::Vec3f colour_vol_; /**< @brief Volumetric colour absorbed by the medium in exponential form.*/
            Entities::Vec3f emission_scat_; /**< @brief Colour emitted by the medium when a ray is scattered.*/
            Entities::Vec3f colour_scat_; /**< @brief Colour reflected by the medium when a ray is scattered.*/
            double scattering_coefficient_; /**< @brief Scattering probability in exponential form.*/
            double order_; /**< @brief Order of the curve describing the distribution of the scattering angle, from 0 to inf. 0 is forced to the scattering angle. Between 0 and 1 is back-scatter, 1 is
                              flat, 1 to inf is forward scatter, 2 is parabole, and inf is forward, not changing the direction.*/
            double scattering_angle_; /**< @brief Portion of the sphere around the intersection point in which the ray will be reflected, from 0 to 1. 0 is forward, not changing the direction, 1 is
                                         all the sphere.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Defines the interaction between a ray and the medium.
             *
             * In this medium, light is absorbed and/or emitted logarithmically with regards to the distance travelled within it by a ray according to the Beer-Lambert law.
             * The ray can be scattered, in which case its origin is set to the point along its path at which it has been scattered, and its direction is set randomly in
             * a portion of the sphere around this point, with the portion being represented by the scattering angle, and the probability distribution within the portion
             * being governed by the order parameter. If scattered, the ray's mask is attenuated with the medium's scattering colour to model part of the light being absorbed.
             * Then, the medium's scattering emissive colour is multiplied with the ray's mask and added to the ray's colour to model light being emitted by the medium.
             *
             * @param ray Ray modified by the medium. Its colour, mask, origin and direction can be changed.
             * @return true Returns true if the ray has been scattered, meaning that its origin and/or direction has changed and the material bounce should not be performed.
             * @return false Returns false when the ray's path has not been changed, and it should bounce on the intersected material as planned.
             */
            auto scatter(Entities::Ray_t& ray) -> bool final;
    };
}}
#endif