#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QObject>
#include <stdio.h>
#include <string>
#include <iostream>
#include "inc-pub/pubSysCls.h"

#include <fstream>
#include <ctime>
#include <cmath>
#include <limits>

#include "basemotor.h"
#include "cartthread.h"

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
    int TEST_TIME = 0;
    int TONE_AMP[3] = {0, 0, 0};
    int TONE_PERIOD[3] = {2, 2, 2};
    SysManager* myMgr;
    

public slots:

    // Access to private variables
    void set_test_time(int);
    void set_tone_amp(int, int);
    void set_tone_period(int, int);
    int  get_test_time();
    int  get_tone_amp(int);
    int  get_tone_period(int);

    // Motor control functions
    int  motor_init();
    int  motor_enable();
    int  motor_disable();
    int  motor_homing();
    int  motor_clear_exception();
    int  motor_multi_tone();
    int  motor_port_close();
};

#endif // MAINCONTROL_H
