#ifndef APTRACER_PORTALSCATTERERTOP_T_H
#define APTRACER_PORTALSCATTERERTOP_T_H

#include "entities/Medium_t.h"
#include <list>

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
}}

using APTracer::Entities::Medium_t;
using APTracer::Entities::TransformMatrix_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The portal scatterer top class is an interface for portal mediums, defining the base they need to move rays around.
     * 
     * This class contains a transformation matrix, which determines how rays are transformed when scattered by the portal.
     * It also contains a medium list, to determine in which mediums are the rays after being transformed.
     */
    class PortalScattererTop_t : public Medium_t{
        public:
            /**
             * @brief Construct a new PortalScattererTop_t object with a transformation matrix and a medium list.
             * 
             * @param transformation Transformation matrix which transforms the rays on hit, potentially moving them to another location. 
             * @param medium_list List of materials in which the portal end is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             * @param ind Refractive index of the medium.
             * @param priority Priority of the medium over other mediums, used to determine which is the current medium when overlapping. Higher value means higher priority.
             */
            PortalScattererTop_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list, double ind, unsigned int priority);

            /**
             * @brief Destroy the PortalScattererTop_t object. Does nothing.
             */
            virtual ~PortalScattererTop_t();

            TransformMatrix_t* transformation_; /**< @brief Transformation matrix which transforms the rays when scattered, potentially moving them to another location.*/
            std::list<Medium_t*> medium_list_; /**< @brief List of materials in which the portal end is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).*/
    };
}}
#endif