#include "ReflectiveRefractiveNormal_t.h"
#include "Shape_t.h"
#include <cmath>
#include "RandomGenerator_t.h"
#include "Texture_t.h"

#define EPSILON 0.00000001

ReflectiveRefractiveNormal_t::ReflectiveRefractiveNormal_t(const Vec3f &emission, const Vec3f &colour, const Texture_t* normal_map, double ind, unsigned int priority, ScatteringFunction_t* scattering) : 
    Medium_t(ind, priority, scattering), emission_(emission), colour_(colour), normal_map_(normal_map) {}

ReflectiveRefractiveNormal_t::~ReflectiveRefractiveNormal_t(){}

void ReflectiveRefractiveNormal_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    Vec3f tangent;
    Vec3f bitangent;
    Vec3f tangent_weights;
    double tuv[2];
    Vec3f newdir;
    //bool coming_out;

    hit_obj->normal_uv_tangent(ray, uv, tuv, normal, tangent);
    bitangent = normal.cross(tangent);
    tangent_weights = normal_map_->get(tuv) * 2.0 - 1.0;
    normal = (tangent * tangent_weights[0] + bitangent * tangent_weights[1] + normal * tangent_weights[2]).normalize();

    double cosi = ray.direction_.dot(normal);

    if (priority_ >= ray.medium_list_.front()->priority_){ // CHECK also discard if priority is equal, but watch for going out case
        Vec3f n;
        double etai, etat;
        double kr;
        
        if (cosi < 0.0){ // Coming in
            etai = ray.medium_list_.front()->ind_;
            etat = ind_;
            cosi *= -1.0;
            n = normal;
            //coming_out = false;
        }
        else{ // Goind out
            etat = (*std::next(ray.medium_list_.begin()))->ind_;
            etai = ind_;
            n = -normal;
            //coming_out = true;
        }

        const double eta = etai/etat;
        const double sint = eta * std::sqrt(1.0 - cosi * cosi);

        if (sint >= 1.0){
            kr = 1.0;
        }
        else{
            const double cost = std::sqrt(1.0 - sint * sint);
            cosi = std::abs(cosi);
            const double Rs = ((etat * cosi) - (etai * cost))/((etat * cosi) + (etai * cost));
            const double Rp = ((etai * cosi) - (etat * cost))/((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp)/2.0;
        }

        if (unif_(my_rand::rng) > kr){ //|| coming_out){ // refracted. Not sure if should always be refracted when going out.
            const double k = 1.0 - sint*sint;

            //newdir = k < 0 ? Vec3f() : (ray.direction_ * eta + n * (eta * cosi - std::sqrt(k))).normalize(); // k shouldn't be smaller than 0 if sint >= 1
            newdir = (ray.direction_ * eta + n * (eta * cosi - std::sqrt(k))).normalize();

            if (newdir.dot(normal) < 0.0){ // coming in
                ray.origin_ += ray.direction_ * ray.dist_ - normal * EPSILON; // use n or normal?
                if (ray.direction_.dot(normal) < 0.0){
                    ray.add_to_mediums(this);
                }
            }
            else{ // going out
                ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON; // use n or normal?
                ray.remove_from_mediums(this);
            }
        }
        else{ // Reflected
            newdir = ray.direction_ - n * 2.0 * cosi;
            ray.origin_ += ray.direction_ * ray.dist_ + n * EPSILON;            
        }

        ray.colour_ += ray.mask_ * emission_;
        ray.mask_ *= colour_;
    }
    else{
        newdir = ray.direction_;
        if (cosi < 0.0){
            ray.origin_ += ray.direction_ * ray.dist_ - normal * EPSILON;
            ray.add_to_mediums(this);
        }
        else{
            ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
            ray.remove_from_mediums(this);
        }
    }

    ray.direction_ = newdir;
}