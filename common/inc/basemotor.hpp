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

#ifndef BASEMOTOR_HPP
#define BASEMOTOR_HPP

/****************************** Motor Initiate Functions ******************************/

/**
 * @brief send message and wait for newline
 * @param[in] msg: message print to user
 */
void msg_user(const char *msg);

/**
 * @brief initialize the port and report the error if the port fails to initialize
 * @param[in] my_mgr: the pointer to the system reference
 * @return int
 * @retval 1 -> success, 0 -> failed
 */
int port_init(sFnd::SysManager *my_mgr);

/**
 * @brief print the node information
 * @param[in] the_node: the shortcut of the node
 */
void node_info(sFnd::INode &the_node);

/**
 * @brief print the port state
 * @param[in] the_port: the shortcut of the port
 */
void port_info(sFnd::IPort &the_port);

/**
 * @brief enable the node, this program should be executed before any operation on node
 * @param[in] the_node: the shortcut of the node
 */
void node_enable(sFnd::INode &the_node);

/**
 * @brief set the parameters for the node
 * @param[in] the_node: the shortcut of the node
 * @param[in] acc_limit: the limit of acceleration (RPM/s)
 * @param[in] dec_limit: the limit of deceleration (RPM/s)
 * @param[in] vel_limit: the limit of speed (RPM)
 */
void node_config(sFnd::INode &the_node, int acc_limit, int dec_limit, int vel_limit);

/**
 * @brief load the configuration generated from Clear View (important for homing program)
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to system instance
 * @param[in] address: the config file location
 */
void node_load_config(sFnd::INode &the_node, sFnd::SysManager *my_mgr, const char *address);

/****************************** Motor Control Functions ******************************/

/**
 * @brief homing program (the configuration file should be loaded in advance)
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to the system reference
 * @return int
 * @retval 1 -> success, 0 -> failed
 */
int homing(sFnd::INode &the_node, sFnd::SysManager *my_mgr);

/**
 * @brief position control, single-tone testing program, generate a data file
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to the system reference
 */
void pos_control(sFnd::INode &the_node, sFnd::SysManager *my_mgr);

/**
 * @brief velocity control, single-tone testing program, generate a data file
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to the system reference
 * @param[in] time_input: test time
 * @param[in] amp: the pointer to the array contains wave heights
 * @param[in] period: the pointer to the array contains wave periods
 */
void vel_control(sFnd::INode &the_node, sFnd::SysManager *my_mgr, int time_input, int amp, int period);

/**
 * @brief multiple tones motion control through velocity start, generate the data file from the encoder, with the control of rocking
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to the system reference
 * @param[in] len: the length of the input data set (wave amplitude and wave period)
 * @param[in] time_input: test time
 * @param[in] amp: the pointer to the array contains wave heights
 * @param[in] period: the pointer to the array contains wave periods
 * @param[in] rock: whether add rocking motion to this test
 */
void multi_tone(sFnd::INode &the_node, sFnd::SysManager *my_mgr, int len, int time_input, int *amp, int *period, bool rock);

/**
 * @brief Jonswap model as input to control the base motor to mimic the real ocean wave, with rocking mode selection
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to the system reference
 * @param[in] time_input: test time
 * @param[in] fetch_distance: fetch distance for the Jonswap input
 * @param[in] wind_speed: U_10 wind speed at ten meters height above the ocean (unit is knot)
 * @param[in] rock: whether add rocking motion to this test
 */
void Jonswap_tone(sFnd::INode &the_node, sFnd::SysManager *my_mgr, int time_input, int fetch_distance, float wind_speed, bool rock);

/**
 * @brief Jonswap mode control use command line
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to the system reference
 */
void Jonswap_tone_cml(sFnd::INode &the_node, sFnd::SysManager *my_mgr);

/**
 * @brief single tone motion control base on velocity control, ask for user input
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to the system reference
 */
void single_tone_cml(sFnd::INode &the_node, sFnd::SysManager *my_mgr);

/**
 * @brief multi-tone motion control based on velocity control, ask for user input
 * @param[in] the_node: the shortcut of the node
 * @param[in] my_mgr: the pointer to the system reference
 */
void multi_tone_cml(sFnd::INode &the_node, sFnd::SysManager *my_mgr);


#endif

