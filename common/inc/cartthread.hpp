/**
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 * @file       cartthread.hpp
 * @brief      the header file of cartthread.cpp
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

#ifndef CARTTHREAD_HPP
#define CARTTHREAD_HPP
#include <cstdint>

/**
 * @brief Constant angular speed rocking motion within specific time
 * @param[in] time: the test time for rocking motion        
 */
void cart_move(int time);

#endif