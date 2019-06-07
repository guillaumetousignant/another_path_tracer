#ifndef PORTALSCATTERER_T_H
#define PORTALSCATTERER_T_H

#include "PortalScattererTop_t.h"
#include "Ray_t.h"
#include <list>
#include <random>

class TransformMatrix_t;
class Medium_t;

class PortalScatterer_t : public PortalScattererTop_t{
    public:
        PortalScatterer_t(TransformMatrix_t* transformation, double scattering_distance, std::list<Medium_t*> medium_list);
        virtual ~PortalScatterer_t();

        double scattering_coefficient_;
        std::uniform_real_distribution<double> unif_;

        virtual void scatter(Ray_t &ray, bool &intersected); 
};
#endif