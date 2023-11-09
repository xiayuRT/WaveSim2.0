/**
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 * @file       cartthread.cpp
 * @brief      instructions to control the cart motor to provide constant velocity motion
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Aug-23-2023     Xiayu Zeng                      xiayu.zeng@runningtide.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 */

#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>
#include <math.h>
#include <time.h>

#include "../inc/cartmotor.hpp"
#include "../inc/spi.hpp"

// Varaibles
uint16_t amplitude = 0;
uint16_t test_time = 0;  
double   frequency = 0;

// Macros
#define STEP_DIVISION_RATIO 64
#define FULL_STEP 1.8
#define MICRO_STEP (FULL_STEP / STEP_DIVISION_RATIO)
#define SECONDS_TO_MICROSECONDS 1000
#define SECONDS_TO_MILLISECONDS 1000000
#define MICROSECONDS_FROM_SECONDS(SECONDS) (SECONDS * SECONDS_TO_MICROSECONDS)
#define MILLISECONDS_FROM_SECONDS(SECONDS) (SECONDS * SECONDS_TO_MILLISECONDS)


/**
 * @brief Initialize the stepper motor with default value          
 * @param[in] sd: high power stepper driver object   
 */
void stepper_init(high_power_stepper_driver& sd){
    sd.reset_settings();                          
    sd.clear_status();                            
    sd.set_decay_mode(hpsd_decay_mode::AUTO_MIXED);   
    sd.set_current_milliamps_36v4(2500);           
    sd.set_step_mode(hpsd_step_mode::MICRO_STEP_64);  
    sd.enable_driver();                           
}


/**
 * @brief One turn rocking 
 * @param[in]sd: high power stepper driver object 
 */
void wave_perform(high_power_stepper_driver& sd){
    double angle_range = amplitude * 2;
    double time_stamp = 1.0 / frequency;
    double step_num_per_half_cycle = angle_range / MICRO_STEP;
    double step_period =  (double)(MILLISECONDS_FROM_SECONDS(time_stamp / step_num_per_half_cycle) - 8.0) * 0.06;
    sd.set_direction(0);
    for (unsigned int x = 0; x < step_num_per_half_cycle / 2; x++){
        sd.step();
        std::this_thread::sleep_for(std::chrono::microseconds((int)step_period));
    }

    sd.set_direction(1);
    for (unsigned int x = 0; x < step_num_per_half_cycle; x++){
        sd.step();
        std::this_thread::sleep_for(std::chrono::microseconds((int)step_period));
    }

    sd.set_direction(0);
    for (unsigned int x = 0; x < step_num_per_half_cycle / 2; x++){
        sd.step();
        std::this_thread::sleep_for(std::chrono::microseconds((int)step_period));
    }
}


void cart_move(int time){
    std::cout << "Cart test begin!" << std::endl;
    
    amplitude = 60;
    frequency = 2;
    test_time = (uint16_t)time;

    high_power_stepper_driver sd;
    stepper_init(sd); // stepper motor initialization

    auto test_start = std::chrono::steady_clock::now();
    auto test_end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = test_end - test_start;
    do{
        wave_perform(sd);
        test_end = std::chrono::steady_clock::now();
        diff = test_end - test_start;
        std::cout << diff.count() << std::endl;
    }while(diff.count() < (double)test_time);

    sd.disable_driver();

    std::cout << "Cart test end!" << std::endl;
}



