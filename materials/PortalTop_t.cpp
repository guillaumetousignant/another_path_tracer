#include "PortalTop_t.h"
#include "Medium_t.h"
#include "TransformMatrix_t.h"

#define EPSILON 0.00000001

using APTracer::Materials::PortalTop_t;

PortalTop_t::PortalTop_t(TransformMatrix_t* transformation, std::list<Medium_t*> medium_list) 
    : transformation_(transformation), medium_list_(medium_list) {}

PortalTop_t::~PortalTop_t(){}