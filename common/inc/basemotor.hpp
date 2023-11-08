/**
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 * @file       basemotor.hpp
 * @brief      the header file of basemotor.cpp
 * 
 * @note       
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Aug-28-2023     Xiayu Zeng                      xiayu.zeng@runningtide.com
 * 
 * @verbatim
 * ==============================================================================
 * 
 * ==============================================================================
 * @endverbatim
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE**************************
 */

#ifndef _BASEMOTOR
#define _BASEMOTOR

/****************************** Motor Initiate Functions ******************************/
void msgUser(const char *msg);
int port_init(sFnd::SysManager *myMgr);
void node_info(sFnd::INode &theNode);
void port_info(sFnd::IPort &thePort);
void node_enable(sFnd::INode &theNode);
void node_config(sFnd::INode &theNode, int Acc_limit, int Dec_limit, int Vel_limit);
void node_load_config(sFnd::INode &theNode, sFnd::SysManager *myMgr, const char *address);

/****************************** Motor Control Functions ******************************/
int homing(sFnd::INode &theNode, sFnd::SysManager *myMgr);
void pos_control(sFnd::INode &theNode, sFnd::SysManager *myMgr);
void vel_control(sFnd::INode &theNode, sFnd::SysManager *myMgr, int time_input, int amp, int period);
void multi_tone(sFnd::INode &theNode, sFnd::SysManager *myMgr, int len, int time_input, int *amp, int *period, bool rock);
void Jonswap_tone(sFnd::INode &theNode, sFnd::SysManager *myMgr, int time_input, int fetch_distance, float wind_speed, bool rock);
void Jonswap_tone_CML(sFnd::INode &theNode, sFnd::SysManager *myMgr);
void Single_tone_CML(sFnd::INode &theNode, sFnd::SysManager *myMgr);
void Multi_tone_CML(sFnd::INode &theNode, sFnd::SysManager *myMgr);

#endif
