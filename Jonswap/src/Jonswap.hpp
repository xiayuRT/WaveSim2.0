#ifndef _JONSWAP_HPP
#define _JONSWAP_HPP

#include <iostream>
#include <cmath>
#include <vector>

class Jonswap {
public:
    Jonswap(const float fetch, const float U_10);
    Jonswap(const float fetch, const float U_10, float set_time);
    Jonswap(const float fetch, const float U_10, float set_time, float set_freq);
    Jonswap(const float fetch, const float U_10, float set_time, float set_freq, float set_amp);

public:
    // Accessor function to pri
    const std::vector<float>& getPSD() const;
    const std::vector<float>& getFREQ() const;
    const std::vector<float>& getTIME() const;
    const std::vector<float>& getAMP() const;
    const std::vector<float>& getETA() const;
    const std::vector<float>& getSPEED() const;
    // Calculate the limited Jonswap stochastic time serires with time and high cut frequency
    int calLIMITED_ETA(float, float, float);  


private:
    // Constant parameters
    const float g = 9.806;                     // Gravity force
    const float gamma = 3.3;                   // Jonswap Cal intermedium parameter
    const float high_cut = 0.3;                // Pre set high-cut frequency
    const float time = 1200;                   // Pre set time: 20 min 
    const float df = 0.001;                    // Frequency beam (best result from matlab script)
    const float dt = 1.0 / 30;                 // Time gap
    
    // Input parameters
    float U_10;                                // Wind speed, unit is Knots
    float fetch;                               // Fetch distance
    float alpha;                               // Jonswap Cal intermedium parameter
    float omega_p;                             // Jonswap Cal intermedium parameter
    
    // Calculate parameters
    std::vector<float> omega;                  // Jonswap Cal intermedium parameter
    std::vector<float> sigma;                  // Jonswap Cal intermedium parameter
    std::vector<float> freq_set;               // Freqency set
    std::vector<float> phase_Jonswap;          // Random phase set
    std::vector<float> time_set;               // Time set
    std::vector<float> PSD_Jonswap;            // Jonswap power spectrum density
    std::vector<float> AMP_Jonswap;            // Jonswap amplitude
    std::vector<float> ETA_Jonswap;            // Jonswap stochastic time serires
    std::vector<float> SPEED_Jonswap;          // Jonswap based motor input speed

    // Help functions
    void calPSD(float);                        // Calculate the Jonswap PSD with high cut frequency
    void calETA(float, float);                 // Calculate the Jonswap stochastic time serires with time and high cut frequency
    
};

#endif
