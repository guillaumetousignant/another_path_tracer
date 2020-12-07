#ifndef APTRACER_PORTALSCATTERER_T_H
#define APTRACER_PORTALSCATTERER_T_H

#include "PortalScattererTop_t.h"
#include "entities/Ray_t.h"
#include <list>
#include <random>

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Medium_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Medium_t;

namespace APTracer { namespace Materials {
    class PortalScatterer_t final : public PortalScattererTop_t{
        public:
            PortalScatterer_t(TransformMatrix_t* transformation, double scattering_distance, std::list<Medium_t*> medium_list, double ind, unsigned int priority);
            virtual ~PortalScatterer_t() final;

            double scattering_coefficient_;
            std::uniform_real_distribution<double> unif_;

            virtual bool scatter(Ray_t &) final; 
    };
}}
#endif