/**
 * **************************(C) COPYRIGHT 2023 Running Tide**************************
 * @file       maincontrol.h
 * @brief      
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
#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QObject>
#include <stdio.h>
#include <string>
#include <iostream>
#include "../../external/inc-pub/pubSysCls.h"

#include <fstream>
#include <ctime>
#include <cmath>
#include <limits>
#include <vector>


using namespace sFnd;
using namespace std;

class maincontrol : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief maincontrol class for GUI to interface with motor control program
     * @param[in]num: sequence of this class
     * @retval         
     */
    explicit maincontrol(int num, QObject *parent = nullptr);

signals:

private:
    int order;
    int test_time = 5;
    int tone_amp[3] = {0, 0, 0};
    int tone_amp_cnt[3] = {0, 0, 0};
    int tone_period[3] = {2, 2, 2};
    int Jonswap_fetch = 10;
    int Jonswap_wind = 10;
    double Jonswap_psd[499] = {};
    bool rocking = false;
    bool Jonswap = false;
    SysManager* my_mgr;
    

public slots:

    /**
     * @brief Set the test time
     * @param[in] val: test time      
     */
    void set_test_time(int);


    /**
     * @brief Set the tone amplitudes
     * @param[in] num: the index tone amplitude array   
     * @param[in] val: amplitude
     */
    void set_tone_amp(int, int);


    /**
     * @brief Set the tone periods          
     * @param[in] num: the index tone periods array   
     * @param[in] val: tone period         
     */  
    void set_tone_period(int, int);


    /**
     * @brief Set the fetch distance           
     * @param[in]val: fetch distance      
     */
    void set_jonswap_fetch(int);


    /**
     * @brief Set the wind speed           
     * @param[in]val: the wind speed in knots         
     */
    void set_jonswap_wind(int);


    /**
     * @brief Set whether use rocking
     * @param[in]judge: bool type to judge whether use rocking        
     */
    void set_rock_status(bool);


    /**
     * @brief Set whether use jonswap
     * @param[in]judge: bool type to judge whether use jonswap        
     */
    void set_jonswap_status(bool);


    /**
     * @brief get the PSD elements          
     * @param[in]num: index of the PSD array 
     * @return double
     * @retval value of the PSD array        
     */
    double get_psd(int);


    /**
     * @brief Get the test time  
     * @return int
     * @retval test time        
     */ 
    int  get_test_time();


    /**
     * @brief Get the tone amplitudes
     * @param[in]num: the index of tone amplitude array
     * @return int
     * @retval the amplitude of specific index tone        
     */
    int  get_tone_amp(int);


    /**
     * @brief Get the tone periods
     * @param[in]num: the index of tone period array
     * @return int
     * @retval the period of specific index tone          
     */
    int  get_tone_period(int);


    /**
     * @brief Get the jonswap fetch distance
     * @return int
     * @retval jonswap fetch distance        
     */
    int  get_jonswap_fetch();


    /**
     * @brief Get the jonswap wind speed
     * @return int
     * @retval jonswap wind speed        
     */
    int  get_jonswap_wind();


    /**
     * @brief Get the jonswap status
     * @return bool
     * @retval whether use Jonswap mode        
     */
    bool get_jonswap_status();

    
    /**
     * @brief Motor initialization
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg     
     */
    int  motor_init();

    
    /**
     * @brief Enable the motor, there will be an atuomatic homing program
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg          
     */
    int  motor_enable();

    
    /**
     * @brief Disable the motor and close the node
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg              
     */
    int  motor_disable();

    
    /**
     * @brief Homing program, the torque feedback is set in configuration
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg    
     */
    int  motor_homing();

    
    /**
     * @brief Clear exceptions, use this after e_stop
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg           
     */
    int  motor_clear_exception();


    /**
     * @brief Close the communication hub port
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg    
     */
    int  motor_port_close();

    
    /**
     * @brief Jonswap mode simulator test
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg    
     */
    int  motor_jonswap();

    
    /**
     * @brief Multiple tone & single tone tests
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg    
     */
    int  motor_multi_tone();

    
    /**
     * @brief Update the Jonswap PSD plot parameter
     * @return int
     * @retval 1-> success, 2,3-> failed, print err msg    
     */
    void plot_para_update();
};

#endif // MAINCONTROL_H
