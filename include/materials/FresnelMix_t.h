#ifndef APTRACER_FRESNELMIX_T_H
#define APTRACER_FRESNELMIX_T_H

#include "entities/MaterialMix_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class Matrial_t;
    class Shape_t;
}}

using APTracer::Entities::MaterialMix_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Material_t;

namespace APTracer { namespace Materials {
    
    /**
     * @brief The Fresnel mix material describes a mix of two materials, with the material being chosen randomly on bounce according to the Fresnel equations.
     * 
     * This material will bounce rays on either of its material randomly according to the proportion
     * of light reflected and refracted according to the Fresnel equations. This means that more light
     * will be bounced by the "reflected" material at grazing angles.
     * This material models surfaces that partly reflect light, like surfaces with a laquer coating,
     * plastic, etc. Reflective and refractive surfaces like glass and water should use the dedicated
     * ReflectiveRefractive materials as they are faster.
     */
    class FresnelMix_t final : public MaterialMix_t{
        public:
            /**
             * @brief Construct a new FresnelMix_t object from a refracted and reflected material, and a refractive index.
             * 
             * @param material_refracted Material that will be bounced in proportion to the refracted part of light according to the Fresnel equations.
             * @param material_reflected Material that will be bounced in proportion to the reflected part of light according to the Fresnel equations.
             * @param ind Refractive index of the material.
             */
            FresnelMix_t(Material_t* material_refracted, Material_t* material_reflected, double ind);
            
            /**
             * @brief Destroy the FresnelMix_t object. Does nothing.
             */
            virtual ~FresnelMix_t() final;

            double ind_; /**< @brief Refractive index of the material.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * The ray is bounced randomly on either the reflected or refracted material according
             * to their proportion calculated from the Fresnel equations. This means that more light 
             * will be bounced by the reflected material at grazing angles, and more light will be
             * bounced by the refracted material when rays are close to the surface normal.
             * 
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif