#ifndef ISOCAMMOTIONBLUR_T_H
#define ISOCAMMOTIONBLUR_T_H

#include "entities/Camera_t.h"
#include "entities/Vec3f.h"
#include <string>
#include <list>
#include <random>

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Skybox_t;
    class Scene_t;
    class Medium_t;
    class ImgBuffer_t;
}}

using APTracer::Entities::Camera_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::ImgBuffer_t;

namespace APTracer { namespace Cameras {
    class IsoCamMotionblur_t final : public Camera_t{
        public:
            IsoCamMotionblur_t(TransformMatrix_t* transformation, const std::string &filename, Vec3f up, const double (&fov)[2], const unsigned int (&subpix)[2], ImgBuffer_t* image, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double (&time)[2], double gammaind);
            virtual ~IsoCamMotionblur_t() final;

            ImgBuffer_t* image_;
            std::uniform_real_distribution<double> unif_;
            Vec3f direction_last_;
            Vec3f origin_last_;
            double time_[2];
            Vec3f up_last_;

            virtual void update() final;
            virtual void raytrace(const Scene_t* scene) final;
            virtual void write(std::string file_name = "") final;
            virtual void show() const final;
            virtual void reset() final;
    };
}}
#endif