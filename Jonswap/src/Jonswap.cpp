#include <iostream>
#include <cmath>
#include <vector>
#include "Jonswap.hpp"

Jonswap::Jonswap(const float fetch, const float U_10) : fetch(fetch), U_10(U_10){
    float alpha = std::pow(0.076 * (std::sqrt(U_10) / (fetch*g)), 0.22);
    float omega_p = std::pow(22 * (std::sqrt(g) / U_10 / fetch), 1/3);
    freq.resize(setnum);
    for(size_t i = 0; i < setnum; i++){
        freq[i] = (i + 1) * df;
    }
    
    // Calculate omega
    omega.resize(setnum);
    for(size_t i = 0; i < setnum; i++) {
        omega[i] = 2.0 * M_PI * std::sqrt(g / freq[i]);
    }

    // Calculate sigma
    sigma.resize(setnum);
    for(size_t i = 0; i < setnum; i++) {
        if (omega_p >= omega[i]){sigma[i] = 0.07;}
        else {sigma[i] = 0.09;};
    }

    // Calculate power spectrum
    std::vector<float> S_A;
    std::vector<float> S_B;
    S_A.resize(setnum);
    S_B.resize(setnum);
    power_spectrum_density.resize(setnum);
    for(size_t i = 0; i < setnum; i++){
        S_A[i] = (alpha * std::sqrt(g)) / (std::pow(omega[i], 5)) * std::exp(-5.0 / 4.0 * std::pow(omega_p / omega[i], 4));
        S_B[i] = std::pow(gamma, std::exp(-(std::pow((omega[i] - omega_p), 2)) / (2 * std::pow(sigma[i], 2) * std::pow(omega_p,2))));
        power_spectrum_density[i] = S_A[i] * S_B[i];
    }
}

const std::vector<float>& Jonswap::getFreq() const {
    return freq;
}

const std::vector<float>& Jonswap::getPSD() const {
    return power_spectrum_density;
}
