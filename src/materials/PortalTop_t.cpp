#include "materials/PortalTop_t.h"
#include "entities/Medium_t.h"
#include "entities/TransformMatrix_t.h"

APTracer::Materials::PortalTop_t::PortalTop_t(APTracer::Entities::TransformMatrix_t* transformation, std::list<APTracer::Entities::Medium_t*> medium_list) 
    : transformation_(transformation), medium_list_(medium_list) {}

APTracer::Materials::PortalTop_t::~PortalTop_t() {}