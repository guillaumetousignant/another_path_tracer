#ifndef PORTAL_T_H
#define PORTAL_T_H

#include "Material_t.h"
#include <list>

class Shape_t;
class TransformMatrix_t;
class Medium_t;

class Portal_t : public Material_t{
    public:
        Portal_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list);
        ~Portal_t();

        TransformMatrix_t* transformation_;
        std::list<Medium_t*> medium_list_;


        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif