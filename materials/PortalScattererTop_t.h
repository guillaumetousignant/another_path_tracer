#ifndef PORTALSCATTERERTOP_T_H
#define PORTALSCATTERERTOP_T_H

#include "ScatteringFunction_t.h"
#include <list>

using APTracer::Entities::ScatteringFunction_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Medium_t;

class TransformMatrix_t;
class Medium_t;

namespace APTracer::Materials {
    class PortalScattererTop_t : public ScatteringFunction_t{
        public:
            PortalScattererTop_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);
            virtual ~PortalScattererTop_t();

            TransformMatrix_t* transformation_;
            std::list<Medium_t*> medium_list_;
    };
}
#endif