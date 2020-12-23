#include "materials/PortalScattererTop_t.h"
#include "entities/TransformMatrix_t.h"
#include "entities/Medium_t.h"

APTracer::Materials::PortalScattererTop_t::PortalScattererTop_t(APTracer::Entities::TransformMatrix_t* transformation, std::list<APTracer::Entities::Medium_t*> medium_list, double ind, unsigned int priority)
    : Medium_t(ind, priority), transformation_(transformation), medium_list_(medium_list) {}

APTracer::Materials::PortalScattererTop_t::~PortalScattererTop_t() = default;