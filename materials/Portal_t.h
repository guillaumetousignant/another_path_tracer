#ifndef PORTAL_T_H
#define PORTAL_T_H

#include "PortalTop_t.h"
#include "Ray_t.h"
#include <list>

class Shape_t;
class TransformMatrix_t;
class Medium_t;

class Portal_t : public PortalTop_t{
    public:
        Portal_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);
        virtual ~Portal_t();

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif