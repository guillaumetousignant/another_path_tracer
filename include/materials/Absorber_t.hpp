#ifndef APTRACER_MATERIALS_ABSORBER_T_HPP
#define APTRACER_MATERIALS_ABSORBER_T_HPP

#include "entities/Medium_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Materials {

    /**
     * @brief The absorber class describes a medium that can absorb and emit light as a ray goes through it.
     *
     * Light is absorbed and/or emitted logarithmically with regards to the distance travelled within it by a ray,
     * according to the Beer-Lambert law.
     * The ray will not change direction when interacting with this medium.
     * This medium can be used to represent light interacting media like light fog, atmospheric scattering, or
     * clear water.
     */
    class Absorber_t final : public Entities::Medium_t {
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
            Absorber_t(Entities::Vec3f emi_vol, Entities::Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double ind, unsigned int priority);

            Entities::Vec3f emission_vol_; /**< @brief Volumetric colour emitted by the medium in exponential form.*/
            Entities::Vec3f colour_vol_; /**< @brief Volumetric colour absorbed by the medium in exponential form.*/

            /**
             * @brief Defines the interaction between a ray and the medium.
             *
             * In this medium, light is absorbed and/or emitted logarithmically with regards to the distance travelled within it by a ray according to the Beer-Lambert law.
             *
             * @param ray Ray modified by the medium. Its colour and mask can be changed.
             * @return true Returns true if the ray has been scattered, meaning that its origin and/or direction has changed and the material bounce should not be performed. Never the case for an
             * absorber.
             * @return false Returns false when the ray's path has not been changed, and it should bounce on the intersected material as planned. Always the case for an absorber.
             */
            auto scatter(Entities::Ray_t& ray) -> bool final;
    };
}}
#endif