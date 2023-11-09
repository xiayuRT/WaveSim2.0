/**
 * **************************(C) COPYRIGHT 2023 Running Tide**************************
 * @file       maincontrol.cpp
 * @brief      main control class for GUI interface with c++ code
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


maincontrol::maincontrol(int num, QObject *parent) : QObject(parent)
{	
    my_mgr = SysManager::Instance();
    order = num;
}


int maincontrol::motor_init()
{   
    try{
        if(!port_init(my_mgr)){return 0;}
        node_load_config(my_mgr->Ports(0).Nodes(0), my_mgr, "/home/wave/Desktop/config.mtr");
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


int maincontrol::motor_enable()
{
    try{
        node_enable(my_mgr->Ports(0).Nodes(0));
        if(!homing(my_mgr->Ports(0).Nodes(0), my_mgr)){
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


int maincontrol::motor_disable()
{
    try{
        my_mgr->Ports(0).Nodes(0).EnableReq(false);
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


int maincontrol::motor_homing()
{
    try{
        if(!homing(my_mgr->Ports(0).Nodes(0), my_mgr)){
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


int maincontrol::motor_clear_exception()
{
    try{
        my_mgr->Ports(0).Nodes(0).Status.AlertsClear();
        node_enable(my_mgr->Ports(0).Nodes(0));
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


int maincontrol::motor_multi_tone()
{	
    try{
        multi_tone(my_mgr->Ports(0).Nodes(0), my_mgr, 3, test_time, tone_amp_cnt, tone_period, rocking );
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


int maincontrol::motor_jonswap()
{
	try{
        Jonswap_tone(my_mgr->Ports(0).Nodes(0), my_mgr, test_time, Jonswap_fetch * KM_TO_M , Jonswap_wind, rocking);
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
        my_mgr->PortsClose();
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


void maincontrol::plot_para_update(){
    // Generate Jonswap data set
    Jonswap jonswap(Jonswap_fetch * KM_TO_M , Jonswap_wind, 40, 0.5);

    const std::vector<float>& psd = jonswap.get_psd();
    for(size_t i = 0; i < 499; i++){
        Jonswap_psd[i] = psd[i];
    }

}


/**************************************************************************ACCESS TO PRIVATE VARIABLES***************************************************************************/

void maincontrol::set_test_time(int val)
{
    test_time = val;
}


void maincontrol::set_tone_amp(int num, int val)
 {
    tone_amp[num] = val;
    tone_amp_cnt[num] = CM_TO_CNT(val);
 }


void maincontrol::set_tone_period(int num, int val)
{
    tone_period[num] = val;
}


void maincontrol::set_jonswap_fetch(int val)
{
	Jonswap_fetch = val;
}


void maincontrol::set_jonswap_wind(int val)
{
	Jonswap_wind = val;
}


void maincontrol::set_rock_status(bool judge)
{
	rocking = judge;
}


void maincontrol::set_jonswap_status(bool judge)
{
	Jonswap = judge;
}


int maincontrol::get_test_time()
{
    return test_time;
};


int maincontrol::get_tone_amp(int num)
{
    return tone_amp[num];
}


int maincontrol::get_tone_period(int num)
{
    return tone_period[num];
}


int maincontrol::get_jonswap_fetch()
{
    return Jonswap_fetch;
}


int maincontrol::get_jonswap_wind()
{
    return Jonswap_wind;
}


bool maincontrol::get_jonswap_status()
{
	return Jonswap;
}


double maincontrol::get_psd(int num){
    return Jonswap_psd[num];
}
