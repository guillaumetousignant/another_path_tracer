#include "Slerp.h"

#define PI 3.141592653589793238463

double APTracer::slerp(double phi_1, double phi_2, double t) {
    phi_2 = phi_2 * t + (phi_1 + 2.0 * PI * ((phi_2 - phi_1) > PI)) * (1.0 - t);
    return phi_2 - 2.0 * PI * (phi_2 > (2.0 * PI));
}
