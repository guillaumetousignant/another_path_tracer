#include "PortalTop_t.h"
#include "Medium_t.h"
#include "TransformMatrix_t.h"

#define EPSILON 0.00000001

APTracer::Materials::PortalTop_t::PortalTop_t(APTracer::Entities::TransformMatrix_t* transformation, std::list<APTracer::Entities::Medium_t*> medium_list) 
    : transformation_(transformation), medium_list_(medium_list) {}

APTracer::Materials::PortalTop_t::~PortalTop_t(){}