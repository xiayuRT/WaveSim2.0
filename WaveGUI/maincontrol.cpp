#include <stdio.h>
#include <string>
#include <iostream>
#include "inc-pub/pubSysCls.h"

#include <fstream>
#include <ctime>
#include <cmath>
#include <limits>

#include "maincontrol.h"
#include "basemotor.h"
#include "cartthread.h"

using namespace sFnd;
using namespace std;

maincontrol::maincontrol(int num, QObject *parent) : QObject(parent)
{
    myMgr= SysManager::Instance();
    ORDER = num;
}

// Motor Initialization
int maincontrol::motor_init()
{   
    try{
        if(!port_init(myMgr)){return 0;}
        node_load_config(myMgr->Ports(0).Nodes(0), myMgr, "/home/wave/Desktop/subscale_test.mtr");
        return 1;
    }
	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		return(2);
	}
	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		return(3);
	}
}

// Enable the motor, there will be an atuomatic homing program
int maincontrol::motor_enable()
{
    try{
        node_enable(myMgr->Ports(0).Nodes(0));
        if(!homing(myMgr->Ports(0).Nodes(0), myMgr)){
            return 0;
        }
        return 1;
    }
	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		return(2);
	}
	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		return(3);
	}
}

// Disable the motor, close the node
int maincontrol::motor_disable()
{
    try{
        myMgr->Ports(0).Nodes(0).EnableReq(false);
        return 1;
    }
	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		return(2);
	}
	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		return(3);
	}
}

// Homing program
int maincontrol::motor_homing()
{
    try{
        if(!homing(myMgr->Ports(0).Nodes(0), myMgr)){
            return 0;
        }
        return 1;
    }
	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		return(2);
	}
	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		return(3);
	}
}

// Clear the motor exceptions
int maincontrol::motor_clear_exception()
{

    try{
        myMgr->Ports(0).Nodes(0).Status.AlertsClear();
        return 1;
    }
	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		return(2);
	}
	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		return(3);
	}
}

// Multiple tone & single tone tests
int maincontrol::motor_multi_tone()
{
    try{
        multi_tone(myMgr->Ports(0).Nodes(0), myMgr, 3, TEST_TIME, TONE_AMP, TONE_PERIOD);
        return 1;
    }
	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		return(2);
	}
	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		return(3);
	}
}

int maincontrol::motor_port_close()
{
    try{
        myMgr->PortsClose();
        return 1;
    }
	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		return(2);
	}
	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		return(3);
	}
}

// Set the test time
void maincontrol::set_test_time(int val)
{
    TEST_TIME = val;
}

// Set the tone amplitudes
void maincontrol::set_tone_amp(int num, int val)
 {
    TONE_AMP[num] = val;
 }

// Set the tone periods
void maincontrol::set_tone_period(int num, int val)
{
    TONE_PERIOD[num] = val;
}

// Get the test time
int maincontrol::get_test_time()
{
    return TEST_TIME;
};

// Get the tone amplitudes
int maincontrol::get_tone_amp(int num)
{
    return TONE_AMP[num];
}

// Get the tone periods
int maincontrol::get_tone_period(int num)
{
    return TONE_PERIOD[num];
}


