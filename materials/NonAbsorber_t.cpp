#include "NonAbsorber_t.h"

using APTracer::Materials::NonAbsorber_t;

NonAbsorber_t::NonAbsorber_t() {}

NonAbsorber_t::~NonAbsorber_t() {}

bool NonAbsorber_t::scatter(Ray_t &ray) {
    return false;
}