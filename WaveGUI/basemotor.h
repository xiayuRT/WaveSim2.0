/**
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 * @file       BaseMotor.hpp
 * @brief      The header file fot the BaseMotor.cpp
 *
 * @note
 * @history:
 *   Version   Date            Author          Modification    Email
 *   V1.0.0    Aug-28-2023     Xiayu Z                         xiayu.zeng@runningtide.com
 *
 * @verbatim
 * ==============================================================================
 *
 * ==============================================================================
 * @endverbatim
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 */

#ifndef _BASEMOTOR
#define _BASEMOTOR

/****************************** Motor Initiate Functions ******************************/
void msgUser(const char *msg);
int  port_init(sFnd::SysManager* myMgr);
void node_info(sFnd::INode& theNode);
void port_info(sFnd::IPort& thePort);
void node_enable(sFnd::INode& theNode);
void node_config(sFnd::INode& theNode, int Acc_limit, int Dec_limit, int Vel_limit);
void node_load_config(sFnd::INode& theNode, sFnd::SysManager* myMgr, const char * address);

/****************************** Motor Control Functions ******************************/
int  homing(sFnd::INode& theNode, sFnd::SysManager* myMgr);
void vel_control(sFnd::INode& theNode, sFnd::SysManager* myMgr, int time_input, int amp, int period);
void pos_control(sFnd::INode& theNode, sFnd::SysManager* myMgr);
void multi_tone(sFnd::INode& theNode, sFnd::SysManager* myMgr, int len, int time_input, int *amp, int *period);
void SingleTone(sFnd::INode& theNode, sFnd::SysManager* myMgr);
void MultiTone(sFnd::INode& theNode, sFnd::SysManager* myMgr);


#endif

