#include "materials/ReflectiveRefractiveNormal_t.hpp"
#include "entities/RandomGenerator_t.hpp"
#include "entities/Shape_t.hpp"
#include <cmath>

constexpr double epsilon = 0.00000001;

using APTracer::Entities::Vec3f;

APTracer::Materials::ReflectiveRefractiveNormal_t::ReflectiveRefractiveNormal_t(const Entities::Vec3f& emission,
                                                                                const Vec3f& colour,
                                                                                const APTracer::Entities::Texture_t* normal_map,
                                                                                APTracer::Entities::Medium_t* medium) :
        emission_(emission), colour_(colour), normal_map_(normal_map), medium_(medium) {}

auto APTracer::Materials::ReflectiveRefractiveNormal_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    Vec3f tangent;
    std::array<double, 2> tuv{};
    Vec3f newdir;
    // bool coming_out;

    Vec3f normal                = hit_obj->normal_uv_tangent(ray.time_, uv, tuv, tangent);
    const Vec3f bitangent       = normal.cross(tangent);
    const Vec3f tangent_weights = normal_map_->get(tuv) * 2.0 - 1.0;
    normal                      = (tangent * tangent_weights[0] + bitangent * tangent_weights[1] + normal * tangent_weights[2]).normalize_inplace();

    double cosi = ray.direction_.dot(normal);

    if (medium_->priority_ >= ray.medium_list_.front()->priority_) { // CHECK also discard if priority is equal, but watch for going out case
        Vec3f n;
        double etai{};
        double etat{};
        double kr{};

        if (cosi < 0.0) { // Coming in
            etai = ray.medium_list_.front()->ind_;
            etat = medium_->ind_;
            cosi *= -1.0;
            n = normal;
            // coming_out = false;
        }
        else { // Going out
            etat = (*std::next(ray.medium_list_.begin()))->ind_;
            etai = medium_->ind_;
            n    = -normal;
            // coming_out = true;
        }

        const double eta  = etai / etat;
        const double sint = eta * std::sqrt(1.0 - cosi * cosi);

        if (sint >= 1.0) {
            kr = 1.0;
        }
        else {
            const double cost = std::sqrt(1.0 - sint * sint);
            cosi              = std::abs(cosi);
            const double Rs   = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            const double Rp   = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr                = (Rs * Rs + Rp * Rp) / 2.0;
        }

        if (unif_(APTracer::Entities::rng()) > kr) { //|| coming_out) { // refracted. Not sure if should always be refracted when going out.
            const double k = 1.0 - sint * sint;

            // newdir = k < 0 ? Vec3f() : (ray.direction_ * eta + n * (eta * cosi - std::sqrt(k))).normalize_inplace(); // k shouldn't be smaller than 0 if sint >= 1
            newdir = (ray.direction_ * eta + n * (eta * cosi - std::sqrt(k))).normalize_inplace();

            if (newdir.dot(normal) < 0.0) { // coming in
                ray.origin_ += ray.direction_ * ray.dist_ - normal * epsilon; // use n or normal?
                if (ray.direction_.dot(normal) < 0.0) {
                    ray.add_to_mediums(medium_);
                }
            }
            else { // going out
                ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon; // use n or normal?
                ray.remove_from_mediums(medium_);
            }
        }
        else { // Reflected
            newdir = ray.direction_ - n * 2.0 * cosi;
            ray.origin_ += ray.direction_ * ray.dist_ + n * epsilon;
        }

        ray.colour_ += ray.mask_ * emission_;
        ray.mask_ *= colour_;
    }
    else {
        newdir = ray.direction_;
        if (cosi < 0.0) {
            ray.origin_ += ray.direction_ * ray.dist_ - normal * epsilon;
            ray.add_to_mediums(medium_);
        }
        else {
            ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon;
            ray.remove_from_mediums(medium_);
        }
    }

    ray.direction_ = newdir;
}
