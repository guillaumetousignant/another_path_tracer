#include "functions/Slerp.hpp"

constexpr double pi = 3.141592653589793238463;

auto APTracer::slerp(double phi_1, double phi_2, double t) -> double {
    phi_2 = phi_2 * t + (phi_1 + 2.0 * pi * static_cast<double>((phi_2 - phi_1) > pi)) * (1.0 - t);
    return phi_2 - 2.0 * pi * static_cast<double>(phi_2 > (2.0 * pi));
}
