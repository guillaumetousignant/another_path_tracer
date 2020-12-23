#ifndef APTRACER_MATERIALMIX_T_H
#define APTRACER_MATERIALMIX_T_H

#include "entities/Material_t.h"

namespace APTracer { namespace Entities {

    /**
     * @brief The material mix class describes a wrapper for two materials that will intersect one or the other, depending on circumstances.
     * 
     * This interface class is used to bind two materials together, leaving up to the derived classes how to choose which material is used on bounce.
     */
    class MaterialMix_t : public Material_t{
        public:
            /**
             * @brief Construct a new MaterialMix_t object from two materials.
             * 
             * @param material_refracted First material of the mix.
             * @param material_reflected Second material of the mix.
             */
            MaterialMix_t(Material_t* material_refracted, Material_t* material_reflected) 
                : material_refracted_(material_refracted), material_reflected_(material_reflected) {};

            /**
             * @brief Destroy the MaterialMix_t object. Does nothing.
             */
            virtual ~MaterialMix_t() {};

            Material_t* material_refracted_; /**< @brief First material of the mix. Doesn't have to be specifically refracted, though it often is.*/
            Material_t* material_reflected_; /**< @brief Second material of the mix. Doesn't have to be specifically reflected, though it often is.*/     
    };
}}

#endif