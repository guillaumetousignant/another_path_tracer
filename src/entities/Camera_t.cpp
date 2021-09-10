#include "entities/Camera_t.h"
#include "entities/TransformMatrix_t.h"
#include <iostream>
#include <chrono>

using APTracer::Entities::Camera_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Skybox_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Vec3f;

Camera_t::Camera_t(TransformMatrix_t* transformation, std::string filename, Vec3f up, std::array<double, 2> fov, std::array<unsigned int, 2> subpix, std::list<Medium_t*> medium_list, Skybox_t* skybox, unsigned int max_bounces, double gammaind) 
            : transformation_(transformation), filename_(std::move(filename)), fov_{fov}, fov_buffer_{fov}, subpix_{subpix}, medium_list_(std::move(medium_list)), 
            skybox_(skybox), max_bounces_(max_bounces), gammaind_(gammaind), up_(up), up_buffer_(up) {
    origin_ = transformation_->multVec(Vec3f()); 
    direction_ = transformation_->multDir(Vec3f(0.0, 1.0, 0.0));
}

auto Camera_t::accumulate(const Scene_t* scene, unsigned int n_iter) -> void {
    unsigned int n = 0;
    while (n < n_iter) {
        ++n;

        auto t_start = std::chrono::high_resolution_clock::now();
        raytrace(scene);
        auto t_end = std::chrono::high_resolution_clock::now();

        std::cout << "Iteration " << n << " done in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;
    }
}

auto Camera_t::accumulate(const Scene_t* scene) -> void {
    unsigned int n = 0;
    while (true) {
        ++n;

        auto t_start = std::chrono::high_resolution_clock::now();
        raytrace(scene);
        auto t_end = std::chrono::high_resolution_clock::now();

        std::cout << "Iteration " << n << " done in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;
    }
}

auto Camera_t::accumulateWrite(const Scene_t* scene, unsigned int n_iter, unsigned int interval) -> void {
    //std::chrono::steady_clock::time_point t_start, t_end;
    unsigned int n = 0;
    while (n < n_iter) {
        ++n;
        
        auto t_start = std::chrono::high_resolution_clock::now();
        raytrace(scene);
        auto t_end = std::chrono::high_resolution_clock::now();

        std::cout << "Iteration " << n << " done in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

        show();
        if (n%interval == 0) {
            std::cout << "Writing started." << std::endl;
            auto t_start2 = std::chrono::high_resolution_clock::now();
            write();
            auto t_end2 = std::chrono::high_resolution_clock::now();

            std::cout << "Writing done in "
            << std::chrono::duration<double, std::milli>(t_end2-t_start2).count()/1000.0 
            << "s." << std::endl;
        }
    }
}

auto Camera_t::accumulateWrite(const Scene_t* scene, unsigned int interval) -> void {
    //std::chrono::steady_clock::time_point t_start, t_end;
    unsigned int n = 0;
    while (true) {
        ++n;
        
        auto t_start = std::chrono::high_resolution_clock::now();
        raytrace(scene);
        auto t_end = std::chrono::high_resolution_clock::now();

        std::cout << "Iteration " << n << " done in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

        show();
        if (n%interval == 0) {
            std::cout << "Writing started." << std::endl;
            auto t_start2 = std::chrono::high_resolution_clock::now();
            write();
            auto t_end2 = std::chrono::high_resolution_clock::now();

            std::cout << "Writing done in "
            << std::chrono::duration<double, std::milli>(t_end2-t_start2).count()/1000.0 
            << "s." << std::endl;
        }
    }
}

auto Camera_t::accumulateWrite(const Scene_t* scene) -> void {
    //std::chrono::steady_clock::time_point t_start, t_end;
    unsigned int n = 0;
    while (true) {
        ++n;
        
        auto t_start = std::chrono::high_resolution_clock::now();
        raytrace(scene);
        auto t_end = std::chrono::high_resolution_clock::now();

        std::cout << "Iteration " << n << " done in " 
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
            << "s." << std::endl;

        show();
        
        std::cout << "Writing started." << std::endl;
        auto t_start2 = std::chrono::high_resolution_clock::now();
        write();
        auto t_end2 = std::chrono::high_resolution_clock::now();

        std::cout << "Writing done in "
        << std::chrono::duration<double, std::milli>(t_end2-t_start2).count()/1000.0 
        << "s." << std::endl;
    }
}

auto Camera_t::setUp(Vec3f &new_up) -> void {
    up_buffer_ = new_up;
}

auto Camera_t::zoom(std::array<double, 2> fov) -> void {
    fov_buffer_ = fov;
}