#ifndef APTRACER_PORTALTOP_T_H
#define APTRACER_PORTALTOP_T_H

#include "entities/Material_t.h"
#include <list>

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Medium_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Medium_t;

namespace APTracer { namespace Materials {
    class PortalTop_t : public Material_t{
        public:
            PortalTop_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);
            virtual ~PortalTop_t();

            TransformMatrix_t* transformation_;
            std::list<Medium_t*> medium_list_;
    };
}}
#endif