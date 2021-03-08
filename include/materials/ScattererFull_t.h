#ifndef APTRACER_SCATTERERFULL_T_H
#define APTRACER_SCATTERERFULL_T_H

#include "entities/Medium_t.h"
#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include <random>

using APTracer::Entities::Medium_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;

namespace APTracer { namespace Materials {
    class ScattererFull_t final : public Medium_t{
        public:
            ScattererFull_t(Vec3f emi_vol, Vec3f col_vol, Vec3f emi_scat, Vec3f col_scat, double abs_dist_emi, double abs_dist_col, double scat_dist, double ind, unsigned int priority);

            Vec3f emission_vol_;
            Vec3f colour_vol_;
            Vec3f emission_scat_;
            Vec3f colour_scat_;
            double scattering_coefficient_;
            std::uniform_real_distribution<double> unif_;

            virtual auto scatter(Ray_t &ray) -> bool final; 
    };
}}
#endif