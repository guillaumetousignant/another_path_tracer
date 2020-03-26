#include "materials/NonAbsorber_t.h"

APTracer::Materials::NonAbsorber_t::NonAbsorber_t() {}

APTracer::Materials::NonAbsorber_t::~NonAbsorber_t() {}

bool APTracer::Materials::NonAbsorber_t::scatter(APTracer::Entities::Ray_t &ray) {
    return false;
}