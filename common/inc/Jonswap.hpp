/**
 * **************************(C) COPYRIGHT 2023 Running Tide**************************
 * @file       Jonswap.hpp
 * @brief      header file of Jonswap.cpp
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

#ifndef JONSWAP_HPP
#define JONSWAP_HPP

#include <iostream>
#include <cmath>
#include <vector>

class Jonswap {
public:

    /**
     * @brief Use fetch distance, wind speed to build a Jonswap class, default time is 1200s, default high cut frequency is 0.3hz   
     * @param[in]fetch: fetch distance (m)
     * @param[in]U_10: wind speed (knots)
     */
    Jonswap(const float fetch, const float U_10);

    
    /**
     * @brief Use fetch distance, wind speed, time to build a Jonswap class, default high cut frequency is 0.3hz    
     * @param[in]fetch: fetch distance (m)
     * @param[in]U_10: wind speed (knots)
     * @param[in]set_time: time series in total      
     */
    Jonswap(const float fetch, const float U_10, float set_time);

    
    /**
     * @brief Use fetch distance, wind speed, time, high cut frequency to build a Jonswap class
     * @param[in]fetch: fetch distance (m)
     * @param[in]U_10: wind speed (knots)
     * @param[in]set_time: time series in total
     * @param[in]set_freq: high cut off frequency      
     */
    Jonswap(const float fetch, const float U_10, float set_time, float set_freq);

    
    /**
     * @brief Use fetch distance, wind speed, time, high cut frequency, wave amplitude limit to build a Jonswap class
     * @param[in]fetch: fetch distance (m)
     * @param[in]U_10: wind speed (knots)
     * @param[in]set_time: time series in total
     * @param[in]set_freq: high cut off frequency
     * @param[in]set_amp: wave height amplitude limit        
     */
    Jonswap(const float fetch, const float U_10, float set_time, float set_freq, float set_amp);

public:

    /**
     * @brief Access to the Jonswap PSD array          
     * @return const std::vector<float>&
     * @retval the address of the Jonswap PSD array           
     */
    const std::vector<float>& get_psd() const;

    
    /**
     * @brief Access to the frequency array           
     * @return const std::vector<float>&
     * @retval the address of the frequency array        
     */
    const std::vector<float>& get_freq() const;

    
    /**
     * @brief Access to the time set array         
     * @return const std::vector<float>&
     * @retval the address of the time set array        
     */
    const std::vector<float>& get_time() const;

    
    /**
     * @brief Access to the Jonswap AMP array          
     * @return const std::vector<float>&
     * @retval the address of the Jonswap AMP array    
     */
    const std::vector<float>& get_amp() const;

    
    /**
     * @brief Access to the Jonswap ETA array        
     * @return const std::vector<float>&
     * @retval the address of the Jonswap ETA array 
     */
    const std::vector<float>& get_eta() const;

    
    /**
     * @brief Access to the Jonswap based speed     
     * @return const std::vector<float>&
     * @retval the address of the Jonswap based speed
     */
    const std::vector<float>& get_speed() const;


    /**
     * @brief Calculate the limited Jonswap stochastic time serires, it will loop at most 100 times to find the suitable data set
     * @param[in/out/in,out]time: the whole time series
     * @param[in/out/in,out]high_cut_freq: the high cut off frequency
     * @param[in/out/in,out]limit_amplitude: the wave height limitation
     * @return int
     * @retval 0 -> falied, 1 -> success
     */
    int cal_limited_eta(float, float, float);  


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

    
    /**
     * @brief Calculate the Jonswap PSD and amplitude in the frequency domain  
     * @param[in]high_cut_freq: high cut off frequency       
     */
    void cal_psd(float); 

    
    /**
     * @brief Calculate the Jonswap stochastic time serires
     * @param[in]time: the whole time series
     * @param[in]high_cut_freq: the high cut off frequency    
     */                      
    void cal_eta(float, float);                 
    
};

#endif
