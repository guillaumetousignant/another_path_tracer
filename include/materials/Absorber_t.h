#ifndef APTRACER_ABSORBER_T_H
#define APTRACER_ABSORBER_T_H

#include "entities/Medium_t.h"
#include "entities/Ray_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Medium_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The absorber class describes a medium that can absorb or emit light as a ray goes through it.
     * 
     * Light is absorbed and/or emitted logarithmically with regards to the distance travelled within it by a ray.
     * The ray will not change direction when interacting with this medium.
     * This medium can be used to represent light interacting media like light fog, atmospheric scattering, or
     * clear water.
     */
    class Absorber_t final : public Medium_t{
        public:
            /**
             * @brief Construct a new Absorber_t object with a colour, emissive colour, and their respective absorption/emission distance.
             * 
             * The two distances are the distances traveled by a ray in the medium to produce the input emissive and absorptive colour, respectively.
             * 
             * @param emi_vol Colour emitted by the medium when a ray traverses abs_dist_emi within it.
             * @param col_vol Colour of the medium when a ray traverses abs_dist_col within it.
             * @param abs_dist_emi Distance travelled through the medium for emi_vol to be emitted.
             * @param abs_dist_col Distance travelled through the medium for col_vol to be absorbed.
             * @param ind Refractive index of the medium.
             * @param priority Priority of the medium over other mediums, used to determine which is the current medium when overlapping. Higher value means higher priority. 
             */
            Absorber_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double ind, unsigned int priority);

            /**
             * @brief Destroy the Absorber_t object. Does nothing.
             */
            virtual ~Absorber_t() final;

            Vec3f emission_vol_; /**< @brief Colour emitted by the medium in exponential form.*/
            Vec3f colour_vol_; /**< @brief Colour absorbed by the medium in exponential form.*/

            /**
             * @brief Defines the interaction between a ray and the medium.
             * 
             * In this medium, light is absorbed and/or emitted logarithmically with regards to the distance travelled within it by a ray.
             * 
             * @param ray Ray modified by the medium. Its colour and mask can be changed.
             * @return true Returns true if the ray has been scattered, meaning that its origin and/or direction has changed and the material bounce should not be performed. Never the case for an absorber.
             * @return false Returns false when the ray's path has not been changed, and it should bounce on the intersected material as planned. Always the case for an absorber.
             */
            virtual auto scatter(Ray_t &ray) -> bool final; 
    };
}}
#endif