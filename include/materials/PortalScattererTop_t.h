#ifndef PORTALSCATTERERTOP_T_H
#define PORTALSCATTERERTOP_T_H

#include "entities/Medium_t.h"
#include <list>

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
}}

using APTracer::Entities::Medium_t;
using APTracer::Entities::TransformMatrix_t;

namespace APTracer { namespace Materials {
    class PortalScattererTop_t : public Medium_t{
        public:
            PortalScattererTop_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);
            virtual ~PortalScattererTop_t();

            TransformMatrix_t* transformation_;
            std::list<Medium_t*> medium_list_;
    };
}}
#endif