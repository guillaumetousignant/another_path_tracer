#include "materials/ReflectiveRefractiveFuzz_t.h"
#include "entities/Shape_t.h"
#include "entities/Medium_t.h"
#include <cmath>
#include "entities/RandomGenerator_t.h"

constexpr double epsilon = 0.00000001;
constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Materials::ReflectiveRefractiveFuzz_t::ReflectiveRefractiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double order, double diffusivity, Medium_t* medium) : 
    emission_(emission), colour_(colour), order_(order), diffusivity_(diffusivity), medium_(medium) {}

APTracer::Materials::ReflectiveRefractiveFuzz_t::~ReflectiveRefractiveFuzz_t() = default;

void APTracer::Materials::ReflectiveRefractiveFuzz_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    const Vec3f normal = hit_obj->normal(ray.time_, uv);
    double cosi = ray.direction_.dot(normal);

    if (medium_->priority_ >= ray.medium_list_.front()->priority_) { // CHECK also discard if priority is equal, but watch for going out case  
        Vec3f newdir;
        double etai, etat;
        double kr;

        const double rand1 = unif_(APTracer::Entities::rng) * 2.0 * pi;
        const double rand2 = std::pow(unif_(APTracer::Entities::rng), order_) * diffusivity_;
        const double rand2s = std::sqrt(rand2);

        const Vec3f axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

        const Vec3f u = axis.cross(normal).normalize_inplace();
        const Vec3f v = normal.cross(u).normalize_inplace(); // wasn't normalized before

        Vec3f normal_fuzz = (u * std::cos(rand1)*rand2s + v*std::sin(rand1)*rand2s + normal*std::sqrt(1.0-rand2)).normalize_inplace();

        cosi = ray.direction_.dot(normal_fuzz);
        
        if (cosi < 0.0) { // Coming in
            etai = ray.medium_list_.front()->ind_;
            etat = medium_->ind_;
            cosi *= -1.0;
            //coming_out = false;
        }
        else{ // Going out
            etat = (*std::next(ray.medium_list_.begin()))->ind_;
            etai = medium_->ind_;
            normal_fuzz *= -1.0;
            //coming_out = true;
        }

        const double eta = etai/etat;
        const double sint = eta * std::sqrt(1.0 - cosi * cosi);

        if (sint >= 1.0) {
            kr = 1.0;
        }
        else{
            const double cost = std::sqrt(1.0 - sint * sint);
            cosi = std::abs(cosi);
            const double Rs = ((etat * cosi) - (etai * cost))/((etat * cosi) + (etai * cost));
            const double Rp = ((etai * cosi) - (etat * cost))/((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp)/2.0;
        }

        if (unif_(APTracer::Entities::rng) > kr) {// || coming_out) { // refracted. Not sure if should always be refracted when going out.
            const double k = 1.0 - sint*sint;

            //newdir = k < 0 ? Vec3f() : (ray.direction_ * eta + normal_fuzz * (eta * cosi - std::sqrt(k))).normalize_inplace();
            newdir = (ray.direction_ * eta + normal_fuzz * (eta * cosi - std::sqrt(k))).normalize_inplace(); // k shouldn't be smaller than zero if kr is smaller than 1

            if (newdir.dot(normal) < 0.0) { // coming in
                ray.origin_ += ray.direction_ * ray.dist_ - normal * epsilon; // use n or normal?
                if (ray.direction_.dot(normal) < 0.0) {
                    ray.add_to_mediums(medium_);
                }
            }
            else{ // going out
                ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon; // use n or normal?
                ray.remove_from_mediums(medium_);
            }
        }
        else{ // Reflected
            newdir = ray.direction_ - normal_fuzz * 2.0 * cosi;
            ray.origin_ += ray.direction_ * ray.dist_ + normal_fuzz * epsilon; // n or normal_fuzz?          
        }

        ray.colour_ += ray.mask_ * emission_;
        ray.mask_ *= colour_;
        ray.direction_ = newdir;
    }
    else{
        if (cosi < 0.0) {
            ray.origin_ += ray.direction_ * ray.dist_ - normal * epsilon;
            ray.add_to_mediums(medium_);
        }
        else{
            ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon;
            ray.remove_from_mediums(medium_);
        }
    }
}