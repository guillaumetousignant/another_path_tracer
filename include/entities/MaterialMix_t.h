#ifndef MATERIALMIX_T_H
#define MATERIALMIX_T_H

#include "entities/Material_t.h"

namespace APTracer::Entities {
    class MaterialMix_t : public Material_t{
        public:
            MaterialMix_t(Material_t* material_refracted, Material_t* material_reflected) 
                : material_refracted_(material_refracted), material_reflected_(material_reflected) {};
            virtual ~MaterialMix_t(){};

            Material_t* material_refracted_;
            Material_t* material_reflected_;        
    };
}

#endif