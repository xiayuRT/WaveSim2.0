/**
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 * @file       CartThread.cpp
 * @brief      This is the thread to control cart motion
 *
 * @note
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Aug-23-2023     Xiayu Z                         xiayu.zeng@runningtide.com
 *
 * @verbatim
 * ==============================================================================
 *
 * ==============================================================================
 * @endverbatim
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 */

#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>
#include <math.h>
#include <time.h>

#include "cartthread.h"
#include "cartmotor.h"
#include "spi.h"

// Varaibles
const int STEP_DIVISION_RATIO = 64;
const float STEP_FULL = 360.0 / 200.0;
const float STEP_MICRO = STEP_FULL / STEP_DIVISION_RATIO;
uint8_t rec_counter = 0; // counting valid input
uint16_t Amplitude = 0;
uint16_t Angle_range = 0;
uint16_t Test_time = 0;
uint32_t test_start_time = 0;
uint32_t test_elapsed_time = 0;
bool start_flag = true;
float Frequency = 0;
float Parameter = 1.0;

// Macros
#define MICROSECONDS_FROM_SECONDS(SECONDS) (SECONDS * 1000)
#define MILLISECONDS_FROM_SECONDS(SECONDS) (SECONDS * 1000000)
#define Angle_range (Amplitude * 2)
#define Period (1.0 / Frequency)
#define STEP_NUM_PERSEC (Angle_range / STEP_MICRO)
// #define STEP_PERIOD (MILLISECONDS_FROM_SECONDS(Period / STEP_NUM_PERSEC) - 6.0) * Parameter

// wave perform
void wave_perform(HighPowerStepperDriver& sd){
    double step_period =  (double)(MILLISECONDS_FROM_SECONDS(Period / STEP_NUM_PERSEC) - 6.0) * Parameter * 0.6;// * 0.4;
    //step_period *= 1000;
    sd.setDirection(0);
    for (unsigned int x = 0; x < STEP_NUM_PERSEC / 2; x++){
        sd.step();
        std::this_thread::sleep_for(std::chrono::microseconds((int)step_period));
    }

    sd.setDirection(1);
    for (unsigned int x = 0; x < STEP_NUM_PERSEC; x++){
        sd.step();
        std::this_thread::sleep_for(std::chrono::microseconds((int)step_period));
    }

    sd.setDirection(0);
    for (unsigned int x = 0; x < STEP_NUM_PERSEC / 2; x++){
        sd.step();
        std::this_thread::sleep_for(std::chrono::microseconds((int)step_period));
    }
}


// main cart move task
void CartMove(int time){
    std::cout << "Cart test begin!" << std::endl;

    Amplitude = 60;
    Frequency = 0.5;
    Test_time = (uint16_t)time;

    HighPowerStepperDriver sd;
    stepper_init(sd); // stepper motor initialization

    auto test_start = std::chrono::steady_clock::now();
    auto test_end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = test_end - test_start;
    do{
        wave_perform(sd);
        test_end = std::chrono::steady_clock::now();
        diff = test_end - test_start;
        // std::cout << diff.count() << std::endl;
    }while(diff.count() < (double)Test_time);

    sd.disableDriver();

    std::cout << "Cart test end!" << std::endl;
}
