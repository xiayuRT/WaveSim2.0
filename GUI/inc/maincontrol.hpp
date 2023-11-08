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
    explicit maincontrol(int num, QObject *parent = nullptr);

signals:

private:
    int ORDER;
    int TEST_TIME = 5;
    int TONE_AMP[3] = {0, 0, 0};
    int TONE_AMP_CNT[3] = {0, 0, 0};
    int TONE_PERIOD[3] = {2, 2, 2};
    int JONSWAP_FETCH = 10;
    int JONSWAP_WIND = 10;
    double JONSWAP_PSD[499] = {};
    bool ROCKING = false;
    bool JONSWAP = false;
    SysManager* myMgr;
    

public slots:

    // Access to private variables
    void set_test_time(int);
    void set_tone_amp(int, int);
    void set_tone_period(int, int);
    void set_jonswap_fetch(int);
    void set_jonswap_wind(int);
    void set_rock_status(bool);
    void set_jonswap_status(bool);
    double get_PSD(int);

    int  get_test_time();
    int  get_tone_amp(int);
    int  get_tone_period(int);
    int  get_jonswap_fetch();
    int  get_jonswap_wind();
    bool get_jonswap_status();

    // Motor control functions
    int  motor_init();
    int  motor_enable();
    int  motor_disable();
    int  motor_homing();
    int  motor_clear_exception();
    int  motor_port_close();
    int  motor_jonswap();
    int  motor_multi_tone();
    void plot_para_update();
};

#endif // MAINCONTROL_H
