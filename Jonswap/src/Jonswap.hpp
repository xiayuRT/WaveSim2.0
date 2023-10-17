#ifndef _JONSWAP_HPP
#define _JONSWAP_HPP

#include <iostream>
#include <cmath>
#include <vector>

class Jonswap {
public:
    Jonswap(const float fetch, const float U_10);
    
    // Accessor function to pri
    const std::vector<float>& getPSD() const;
    const std::vector<float>& getFreq() const;

private:
    // Constant parameters
    const float g = 9.806;
    const float gamma = 3.3;
    const int setnum = 300;
    const float df = 0.01;
    // Input parameters
    float U_10;
    float fetch;
    std::vector<float> omega;
    std::vector<float> sigma;
    std::vector<float> freq;
    std::vector<float> power_spectrum_density;
};


#endif