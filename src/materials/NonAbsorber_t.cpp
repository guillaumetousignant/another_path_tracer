#include "materials/NonAbsorber_t.h"

APTracer::Materials::NonAbsorber_t::NonAbsorber_t(double ind, unsigned int priority) : Medium_t(ind, priority) {}

auto APTracer::Materials::NonAbsorber_t::scatter(APTracer::Entities::Ray_t& ray) -> bool {
    return false;
}