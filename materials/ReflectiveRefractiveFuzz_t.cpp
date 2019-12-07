#include "ReflectiveRefractiveFuzz_t.h"
#include "Shape_t.h"
#include <cmath>
#include "RandomGenerator_t.h"

#define EPSILON 0.00000001
#define PI 3.141592653589793238463

ReflectiveRefractiveFuzz_t::ReflectiveRefractiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, double order, double diffusivity, ScatteringFunction_t* scattering) : 
    Medium_t(ind, priority, scattering), emission_(emission), colour_(colour), order_(order), diffusivity_(diffusivity) {}

ReflectiveRefractiveFuzz_t::~ReflectiveRefractiveFuzz_t(){}

void ReflectiveRefractiveFuzz_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    Vec3f newdir;
    //bool coming_out;

    hit_obj->normal(ray, uv, normal);
    double cosi = ray.direction_.dot(normal);

    if (priority_ >= ray.medium_list_.front()->priority_){ // CHECK also discard if priority is equal, but watch for going out case
        Vec3f n;
        double etai, etat;
        double kr;
        
        if (cosi < 0.0){ // Coming in
            etai = ray.medium_list_.front()->ind_;
            etat = ind_;
            //cosi *= -1;
            n = normal;
            //coming_out = false;
        }
        else{ // Goind out
            etat = (*std::next(ray.medium_list_.begin()))->ind_;
            etai = ind_;
            n = -normal;
            //coming_out = true;
        }

        const double rand1 = unif_(my_rand::rng)*2.0*PI;
        const double rand2 = std::pow(unif_(my_rand::rng), order_) * diffusivity_;
        const double rand2s = sqrt(rand2);

        const Vec3f axis = std::abs(n[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

        const Vec3f u = axis.cross(normal).normalize_inplace();
        const Vec3f v = normal.cross(u).normalize_inplace(); // wasn't normalized before

        const Vec3f normal_fuzz = (u * std::cos(rand1)*rand2s + v*std::sin(rand1)*rand2s + normal*std::sqrt(1.0-rand2)).normalize_inplace();

        cosi = ray.direction_.dot(normal_fuzz);
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

        if (unif_(my_rand::rng) > kr){// || coming_out){ // refracted. Not sure if should always be refracted when going out.
            const double k = 1.0 - sint*sint;

            //newdir = k < 0 ? Vec3f() : (ray.direction_ * eta + normal_fuzz * (eta * cosi - std::sqrt(k))).normalize_inplace();
            newdir = (ray.direction_ * eta + normal_fuzz * (eta * cosi - std::sqrt(k))).normalize_inplace(); // k shouldn't be smaller than zero if kr is smaller than 1

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
            newdir = ray.direction_ - normal_fuzz * 2.0 * cosi;
            ray.origin_ += ray.direction_ * ray.dist_ + n * EPSILON; // n or normal_fuzz?          
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