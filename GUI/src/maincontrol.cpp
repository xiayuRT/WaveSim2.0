/**
 * **************************(C) COPYRIGHT 2023 Running Tide**************************
 * @file       maincontrol.cpp
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
#include <stdio.h>
#include <string>
#include <iostream>
#include "../../external/inc-pub/pubSysCls.h"

#include <fstream>
#include <ctime>
#include <cmath>
#include <limits>
#include <thread>

#include "../inc/maincontrol.hpp"
#include "../../common/inc/basemotor.hpp"

#include "../../common/inc/Jonswap.hpp"

using namespace sFnd;
using namespace std;

#define KM_TO_M 1000
#define CNT_PER_ROUND 800
#define PULLEY_DIAMETER_CM 4.85
#define CM_TO_CNT(len) ((len) * CNT_PER_ROUND / (M_PI * PULLEY_DIAMETER_CM))

/**
 * @brief maincontrol class for GUI to interface with motor control program
 * @param[in]num: sequence of this class
 * @retval         
 */
maincontrol::maincontrol(int num, QObject *parent) : QObject(parent)
{	
    myMgr = SysManager::Instance();
    ORDER = num;
}


// Motor Initialization
/**
 * @brief Motor initialization
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg     
 */
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


/**
 * @brief Enable the motor, there will be an atuomatic homing program
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg          
 */
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


/**
 * @brief Disable the motor and close the node
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg              
 */
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


/**
 * @brief Homing program, the torque feedback is set in configuration
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg    
 */
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


/**
 * @brief Clear exceptions, use this after e_stop
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg           
 */
int maincontrol::motor_clear_exception()
{
    try{
        myMgr->Ports(0).Nodes(0).Status.AlertsClear();
        node_enable(myMgr->Ports(0).Nodes(0));
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


/**
 * @brief Multiple tone & single tone tests
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg    
 */
int maincontrol::motor_multi_tone()
{	
    try{
        multi_tone(myMgr->Ports(0).Nodes(0), myMgr, 3, TEST_TIME, TONE_AMP_CNT, TONE_PERIOD, ROCKING );
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


/**
 * @brief Jonswap mode simulator test
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg    
 */
int maincontrol::motor_jonswap()
{
	try{
        Jonswap_tone(myMgr->Ports(0).Nodes(0), myMgr, TEST_TIME, JONSWAP_FETCH * KM_TO_M , JONSWAP_WIND, ROCKING);
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


/**
 * @brief Close the communication hub port
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg    
 */
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


/**
 * @brief Update the Jonswap PSD plot parameter
 * @return int
 * @retval 1-> success, 2,3-> failed, print err msg    
 */
void maincontrol::plot_para_update(){
    // Generate Jonswap data set
    Jonswap jonswap(JONSWAP_FETCH * KM_TO_M , JONSWAP_WIND, 40, 0.5);

    const std::vector<float>& psd = jonswap.getPSD();
    for(size_t i = 0; i < 499; i++){
        JONSWAP_PSD[i] = psd[i];
    }

}


/**************************************************************************ACCESS TO PRIVATE VARIABLES***************************************************************************/

// Set the test time
void maincontrol::set_test_time(int val)
{
    TEST_TIME = val;
}

// Access to PSD elements
double maincontrol::get_PSD(int num){
    return JONSWAP_PSD[num];
}

// Set the tone amplitudes
void maincontrol::set_tone_amp(int num, int val)
 {
    TONE_AMP[num] = val;
    TONE_AMP_CNT[num] = CM_TO_CNT(val);
 }

// Set the tone periods
void maincontrol::set_tone_period(int num, int val)
{
    TONE_PERIOD[num] = val;
}

// Set the fetch distance
void maincontrol::set_jonswap_fetch(int val)
{
	JONSWAP_FETCH = val;
}

// Set the wind speed
void maincontrol::set_jonswap_wind(int val)
{
	JONSWAP_WIND = val;
}

// Set whether use rocking
void maincontrol::set_rock_status(bool judge)
{
	ROCKING = judge;
}

// Set whether use jonswap
void maincontrol::set_jonswap_status(bool judge)
{
	JONSWAP = judge;
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

// Get the jonswap fetch distance
int maincontrol::get_jonswap_fetch()
{
    return JONSWAP_FETCH;
}

// Get the jonswap wind speed
int maincontrol::get_jonswap_wind()
{
    return JONSWAP_WIND;
}

// Get the jonswap status
bool maincontrol::get_jonswap_status()
{
	return JONSWAP;
}

