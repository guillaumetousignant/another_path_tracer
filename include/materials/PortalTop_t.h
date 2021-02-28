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

    /**
     * @brief The portal top class is an interface for portal materials, defining the base they need to move rays around.
     * 
     * This class contains a transformation matrix, which determines how rays are transformed when hitting the portal.
     * It also contains a medium list, to determine in which mediums are the rays after being transformed.
     */
    class PortalTop_t : public Material_t{
        public:
            /**
             * @brief Construct a new PortalTop_t object with a transformation matrix and a medium list.
             * 
             * @param transformation Transformation matrix which transforms the rays on hit, potentially moving them to another location. 
             * @param medium_list List of materials in which the portal end is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).
             */
            PortalTop_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);

            /**
             * @brief Destroy the PortalTop_t object. Does nothing.
             */
            virtual ~PortalTop_t();

            TransformMatrix_t* transformation_; /**< @brief Transformation matrix which transforms the rays on hit, potentially moving them to another location.*/
            std::list<Medium_t*> medium_list_; /**< @brief List of materials in which the portal end is placed. Should have at least two copies of an "outside" medium not assigned to any object (issue #25).*/
    };
}}
#endif