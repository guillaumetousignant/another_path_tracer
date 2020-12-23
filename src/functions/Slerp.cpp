#include "functions/Slerp.h"

constexpr double pi = 3.141592653589793238463;

double APTracer::slerp(double phi_1, double phi_2, double t) {
    phi_2 = phi_2 * t + (phi_1 + 2.0 * pi * ((phi_2 - phi_1) > pi)) * (1.0 - t);
    return phi_2 - 2.0 * pi * (phi_2 > (2.0 * pi));
}
