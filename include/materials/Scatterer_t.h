#ifndef APTRACER_MATERIALS_SCATTERER_T_H
#define APTRACER_MATERIALS_SCATTERER_T_H

#include "entities/Medium_t.h"
#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include <random>

using APTracer::Entities::Medium_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The scatterer class describes a medium that can absorb and emit light as a ray goes through it, and that can scatter it.
     * 
     * Light is absorbed and/or emitted logarithmically with regards to the distance travelled within it by a ray,
     * according to the Beer-Lambert law.
     * The ray has a chance to be scattered relative to the distance travelled, where it acts ash though it was
     * refracted by a particle in the medium. Its origin becomes that intersection point, and is is reflected in 
     * a random direction within the sphere around that intersection point.
     * This medium can be used to represent dense interacting media like fog, humid atmospheric scattering, troubled 
     * water, wax, skin, and marble.
     */
    class Scatterer_t final : public Medium_t {
        public:
            /**
             * @brief Construct a new Scatterer_t object with a colour, emissive colour, and their respective absorption/emission distance, and a scattering distance.
             * 
             * The first two distances are the distances traveled by a ray in the medium to produce the input emissive and absorptive colour, respectively.
             * The scattering distance is the average distance at which a ray will be scatterred.
             * 
             * @param emi_vol Colour emitted by the medium when a ray traverses abs_dist_emi within it.
             * @param col_vol Colour of the medium when a ray traverses abs_dist_col within it.
             * @param abs_dist_emi Distance travelled through the medium for emi_vol to be emitted.
             * @param abs_dist_col Distance travelled through the medium for col_vol to be absorbed.
             * @param scat_dist Average distance travelled through the medium at which rays are scattered.
             * @param ind Refractive index of the medium.
             * @param priority Priority of the medium over other mediums, used to determine which is the current medium when overlapping. Higher value means higher priority. 
             */
            Scatterer_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double scat_dist, double ind, unsigned int priority);

            Vec3f emission_vol_; /**< @brief Volumetric colour emitted by the medium in exponential form.*/
            Vec3f colour_vol_; /**< @brief Volumetric colour absorbed by the medium in exponential form.*/
            double scattering_coefficient_; /**< @brief Scattering probability in exponential form.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Defines the interaction between a ray and the medium.
             * 
             * In this medium, light is absorbed and/or emitted logarithmically with regards to the distance travelled within it by a ray according to the Beer-Lambert law.
             * The ray can be scattered, in which case its origin is set to the point along its path at which it has been scattered, and its direction is set randomly in 
             * the sphere around this point.
             * 
             * @param ray Ray modified by the medium. Its colour, mask, origin and direction can be changed.
             * @return true Returns true if the ray has been scattered, meaning that its origin and/or direction has changed and the material bounce should not be performed.
             * @return false Returns false when the ray's path has not been changed, and it should bounce on the intersected material as planned.
             */
            virtual auto scatter(Ray_t &ray) -> bool final; 
    };
}}
#endif