/**
 * **************************(C) COPYRIGHT 2023 Running Tide**************************
 * @file       Jonswap.cpp
 * @brief      Jonswap algorithm developed to generate wave height output in time series
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Oct-26-2023     Xiayu Zeng                      xiayu.zeng@runningtide.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * **************************(C) COPYRIGHT 2023 Running Tide**************************
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include "Jonswap.hpp"


/**
 * @brief Use fetch distance, wind speed to build a Jonswap class, default time is 1200s, default high cut frequency is 0.3hz   
 * @param[in]fetch: fetch distance (m)
 * @param[in]U_10: wind speed (knots)
 */
Jonswap::Jonswap(const float fetch, const float U_10) : fetch(fetch), U_10(U_10){
    calPSD(high_cut);// Calculate power spectrum
    calETA(time, high_cut);// Calculate wave in time series
}


/**
 * @brief Use fetch distance, wind speed, time to build a Jonswap class, default high cut frequency is 0.3hz    
 * @param[in]fetch: fetch distance (m)
 * @param[in]U_10: wind speed (knots)
 * @param[in]set_time: time series in total      
 */
Jonswap::Jonswap(const float fetch, const float U_10, float set_time) : fetch(fetch), U_10(U_10){
    calPSD(high_cut);// Calculate power spectrum
    calETA(set_time, high_cut);// Calculate wave in time series
}


/**
 * @brief Use fetch distance, wind speed, time, high cut frequency to build a Jonswap class
 * @param[in]fetch: fetch distance (m)
 * @param[in]U_10: wind speed (knots)
 * @param[in]set_time: time series in total
 * @param[in]set_freq: high cut off frequency      
 */
Jonswap::Jonswap(const float fetch, const float U_10, float set_time, float set_freq) : fetch(fetch), U_10(U_10){
    calPSD(set_freq);// Calculate power spectrum
    calETA(set_time, set_freq);// Calculate wave in time series
}


/**
 * @brief Use fetch distance, wind speed, time, high cut frequency, wave amplitude limit to build a Jonswap class
 * @param[in]fetch: fetch distance (m)
 * @param[in]U_10: wind speed (knots)
 * @param[in]set_time: time series in total
 * @param[in]set_freq: high cut off frequency
 * @param[in]set_amp: wave height amplitude limit        
 */
Jonswap::Jonswap(const float fetch, const float U_10, float set_time, float set_freq, float set_amp) : fetch(fetch), U_10(U_10){
    calPSD(set_freq);// Calculate power spectrum
    calLIMITED_ETA(set_time, set_freq, set_amp);// Calculate wave in time series
}


/**
 * @brief Calculate the Jonswap PSD and amplitude in the frequency domain  
 * @param[in]high_cut_freq: high cut off frequency       
 */
void Jonswap::calPSD(float high_cut_freq){
    int freqnum = high_cut_freq / df;
    alpha = 0.076 * std::pow((std::pow(U_10, 2) / (fetch*g)), 0.22); // calculate alpha
    omega_p = 22 * std::pow((std::pow(g, 2) / (U_10 * fetch)), 1.0 /3); // calculate omega_p

    // Generate the ferquency set
    freq_set.resize(freqnum);
    for(size_t i = 0; i < freqnum; i++){
        freq_set[i] = (i + 1) * df;
    }
    
    // Calculate the omega set
    omega.resize(freqnum);
    for(size_t i = 0; i < freqnum; i++) {
        omega[i] = 2.0 * M_PI * freq_set[i];
    }

    // Calculate the sigma set
    sigma.resize(freqnum);
    for(size_t i = 0; i < freqnum; i++) {
        if (omega_p >= omega[i]){sigma[i] = 0.07;}
        else {sigma[i] = 0.09;};
    }

    // Init the vector size
    PSD_Jonswap.resize(freqnum);
    std::vector<float> S_A;
    std::vector<float> S_B;
    S_A.resize(freqnum);
    S_B.resize(freqnum);
    for(size_t i = 0; i < freqnum; i++){
        S_A[i] = (alpha * std::pow(g, 2)) / (std::pow(omega[i], 5)) * std::exp(-5.0 / 4.0 * std::pow(omega_p / omega[i], 4));
        S_B[i] = std::pow(gamma, std::exp(-(std::pow((omega[i] - omega_p), 2)) / (2 * std::pow(sigma[i], 2) * std::pow(omega_p,2))));
        PSD_Jonswap[i] = S_A[i] * S_B[i];
    }

    // Calculate the Jonswap AMP
    AMP_Jonswap.resize(freqnum);
    for(size_t i=0; i < freqnum; i++){
        AMP_Jonswap[i] = sqrt(2 * PSD_Jonswap[i] * df);
    } 
}


/**
 * @brief Calculate the Jonswap stochastic time serires
 * @param[in]time: the whole time series
 * @param[in]high_cut_freq: the high cut off frequency    
 */
void Jonswap::calETA(float time, float high_cut_freq){
    // Initialize the number of time beams and frequency beams
    int timenum = time / dt;
    int freqnum = high_cut_freq / df;
    
    // Generate random phase 
    std::random_device rd; // Random number generator seed
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(0.0, 2 * M_PI);
    phase_Jonswap.resize(freqnum);
    for(size_t i = 0; i < freqnum; i++){
        phase_Jonswap[i] = distribution(gen);
    }

    // Generate the time set
    time_set.resize(timenum);
    for(size_t i = 0; i < timenum; i++){
        time_set[i] = (i + 1) * dt;
    }
    
    // Calculate the stochastic time series & Calculate the discrete speed series
    ETA_Jonswap.resize(timenum);
    SPEED_Jonswap.resize(timenum);
    for(size_t i = 0; i < timenum; i++){
        ETA_Jonswap[i] = 0; 
        SPEED_Jonswap[i] = 0;
        for(size_t j = 0; j < freqnum; j++){
            ETA_Jonswap[i] += AMP_Jonswap[j] * cos(2 * M_PI * freq_set[j] * time_set[i] + phase_Jonswap[j]);
        }
        if(i > 0){
            SPEED_Jonswap[i - 1] = (ETA_Jonswap[i] - ETA_Jonswap[i-1])/dt;
        }
    }
}


/**
 * @brief Calculate the limited Jonswap stochastic time serires, it will loop at most 100 times to find the suitable data set
 * @param[in/out/in,out]time: the whole time series
 * @param[in/out/in,out]high_cut_freq: the high cut off frequency
 * @param[in/out/in,out]limit_amplitude: the wave height limitation
 * @return          int
 * @retval          0 -> falied, 1 -> success
 */
int Jonswap::calLIMITED_ETA(float time, float high_cut_freq, float limit_amplitude){
    // Iterate 100 times to generate the amplitude with amplitude limitation
    bool flag = false;
    int counter = 0;
    do{
        flag = false;
        calETA(time, high_cut_freq);
        int timenum = time / dt;
        // Check if there is amplitude over limit 
        for(size_t i = 0; i < timenum; i++){
            if (std::abs(ETA_Jonswap[i]) > limit_amplitude){
                flag = true;
            }
        }
        counter++;
        std::cout << counter << " set generated!" << std::endl;
    }while(flag && counter < 100);

    // Check the result
    if(flag && counter == 100){
        std::cout << "Failed!" << std::endl;
        return 0;}
    else {
        std::cout << "Success!" << std::endl;
        return 1;}
}


// Access to the frequency array
const std::vector<float>& Jonswap::getFREQ() const {
    return freq_set;
}


// Access to the time set array
const std::vector<float>& Jonswap::getTIME() const {
    return time_set;
}


// Access to the Jonswap PSD array
const std::vector<float>& Jonswap::getPSD() const {
    return PSD_Jonswap;
}


// Access to the Jonswap AMP array
const std::vector<float>& Jonswap::getAMP() const {
    return AMP_Jonswap;
}


// Access to the Jonswap ETA array
const std::vector<float>& Jonswap::getETA() const {
    return ETA_Jonswap;
}


// Access to the Jonswap based speed
const std::vector<float>& Jonswap::getSPEED() const{
    return SPEED_Jonswap;
}
