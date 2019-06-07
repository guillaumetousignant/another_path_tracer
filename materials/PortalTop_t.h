#ifndef PORTALTOP_T_H
#define PORTALTOP_T_H

#include "Material_t.h"
#include <list>

class TransformMatrix_t;
class Medium_t;

class PortalTop_t : public Material_t{
    public:
        PortalTop_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);
        virtual ~PortalTop_t();

        TransformMatrix_t* transformation_;
        std::list<Medium_t*> medium_list_;
};
#endif