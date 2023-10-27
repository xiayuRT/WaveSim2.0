/**
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE****************************
 * @file       BaseMotor.cpp
 * @brief      The instructions to control the base motor
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
 * **************************(C) COPYRIGHT 2023 RUNNING TIDE****************************
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include "inc-pub/pubSysCls.h"

#include <fstream>
#include <ctime>
#include <cmath>
#include <limits>

#include "basemotor.hpp"
#include "Jonswap.hpp"

using namespace sFnd;
using namespace std;

#define PULLEY_DIAMETER_CM 4.85
#define CNT_PER_ROUND 800
#define M_TO_CM 100
#define M_PER_SEC_TO_RPM(x) ((x) * 60 * M_TO_CM / (M_PI * PULLEY_DIAMETER_CM)) 
#define M_TO_CNT(x) ((x) * M_TO_CM * CNT_PER_ROUND / (M_PI * PULLEY_DIAMETER_CM))
#define CNT_TO_CM(x) ((x) * M_PI * PULLEY_DIAMETER_CM / CNT_PER_ROUND)


/**
 * @brief send message and wait for newline
 * @param[in]msg: message print to user
 * @retval
 */
void msgUser(const char *msg) {
    std::cout << msg;
    getchar();
}


/**
 * @brief  initilaize the port and report the error if the port fail to initialize
 * @return int
 * @retval 1->success, 0->failed
 */
int port_init(sFnd::SysManager* myMgr){
    size_t portCount = 0;
    std::vector<std::string> comHubPorts;
    SysManager::FindComHubPorts(comHubPorts);
    printf("Found %d SC Hubs\n", comHubPorts.size());

    for (portCount = 0; portCount < comHubPorts.size() && portCount < NET_CONTROLLER_MAX; portCount++) {
        myMgr->ComHubPort(portCount, comHubPorts[portCount].c_str()); 	//define the first SC Hub port (port 0) to be associated
                                        // with COM portnum (as seen in device manager)
    }

    // Only one port used in wave sim system
    if (portCount == 1) {
        myMgr->PortsOpen(portCount);				//Open the port
        cout << "Port is opened successfully!" << endl;
        return 1;
    } else if (portCount > 1){
        cout << "Port num is larger than 1, check SC4 HUB carefully!" << endl;
        msgUser("Press any key to continue."); // pause so the user can see the error message; waits for user to press a key
        return 0;
    } else {
        printf("Unable to locate SC hub port\n");
        msgUser("Press any key to continue."); // pause so the user can see the error message; waits for user to press a key
        return 0;  // This returns error
    }
}


/**
 * @brief Print the node information
 * @param[in]theNode: the shortcut of the Node
 * @retval -
 */
void node_info(sFnd::INode& theNode){
    printf("Node[0]: type=%d\n", theNode.Info.NodeType());
    printf("userID: %s\n", theNode.Info.UserID.Value());
    printf("FW version: %s\n", theNode.Info.FirmwareVersion.Value());
    printf("Serial #: %d\n", theNode.Info.SerialNumber.Value());
    printf("Model: %s\n", theNode.Info.Model.Value());
}


/**
 * @brief Print the port state
 * @param[in]thePort: the shortcut of the Port
 * @retval -
 */
void port_info(sFnd::IPort& thePort){
    printf("Port[%d]: state=%d, nodes=%d\n", thePort.NetNumber(), thePort.OpenState(), thePort.NodeCount());
}


/**
 * @brief enable the Node, this program should be executed before any operation on Node
 * @param[in]theNode: the shortcut of the Node
 * @retval -
 */
void node_enable(sFnd::INode& theNode){
    theNode.Status.AlertsClear();   // clear shutdowns
    theNode.Motion.NodeStopClear(); // clear NodeStops
    theNode.EnableReq(true);        // enable the Node
    printf("Node is enabled.\n");
}


/**
 * @brief set the parameters for the Node
 * @param[in]theNode:   the shortcut of the Node
 * @param[in]Acc_limit: the limit of acceleration (RPM/s)
 * @param[in]Dec_limit: the limit of deceleration (RPM/s)
 * @param[in]Vel_limit: the limit of speed (RPM)
 * @retval -
 */
void node_config(sFnd::INode& theNode, int Acc_limit, int Dec_limit, int Vel_limit){
    theNode.AccUnit(INode::RPM_PER_SEC);       // Set the units for Acceleration to RPM/SEC
    theNode.VelUnit(INode::RPM);               // Set the units for Velocity to RPM
    theNode.Motion.AccLimit = Acc_limit;       // Set the limit of Acceleration
    theNode.Motion.Adv.DecelLimit = Dec_limit; // Set the limit of Deceleration
    theNode.Motion.VelLimit = Vel_limit;       // Set the limit of Velocity
}


/**
 * @brief load the configuration generated from Clear View (important for homing program)
 * @param[in/out/in,out]theNode: the shortcut of the Node
 * @param[in/out/in,out]myMgr:   the pointer to system instance
 * @param[in/out/in,out]address: the config file location
 * @retval -
 */
void node_load_config(sFnd::INode& theNode, sFnd::SysManager* myMgr, const char * address){
    theNode.EnableReq(false);          // ensure Node is disabled before loading config file
    myMgr->Delay(200);
    theNode.Setup.ConfigLoad(address); // load configuration
}


/*****************************************************************************Motor Control*******************************************************************************/


/**
 * @brief homing program (the configuration file should be loaded in advance)
 * @param[in/out/in,out]theNode: the short cut of the node work with
 * @return int
 * @retval 1 -> success, 0 -> failed
 */
int homing(sFnd::INode& theNode, sFnd::SysManager* myMgr){
    double TIME_TILL_TIMEOUT = 10000; // timeout used for status waiting
    double timeout = myMgr->TimeStampMsec() + TIME_TILL_TIMEOUT; // define a timeout in case the node is unable to move
    while (!theNode.Motion.IsReady()) {
        if (myMgr->TimeStampMsec() > timeout) {
            printf("Error: Timed out waiting for Node to enable\n");
            msgUser("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key
            return 0;
        }
    }

    if (theNode.Motion.Homing.HomingValid()){
        if (theNode.Motion.Homing.WasHomed()){
            printf("Node has already been homed, current position is: \t%8.0f \n", theNode.Motion.PosnMeasured.Value());
            printf("Rehoming Node... \n");
        } else {
            printf("Node has not been homed. Homing Node now...\n");
        }

        // Home the Node
        theNode.Motion.Homing.Initiate();
        timeout = myMgr->TimeStampMsec() + TIME_TILL_TIMEOUT;
        while (!theNode.Motion.Homing.WasHomed()){
            if(myMgr->TimeStampMsec() > timeout){
                printf("Node did not completing homing: \n\t -Ensure Homing settings have been defined through ClearView. \n\t -Check for alerts/Shutdowns \n\t -Ensure timeout is longer than the longest possible homing move.\n");
                msgUser("Press any key to continue.");
                return 0;
            }
        }
        printf("Node completed homing\n");
        printf("Node has already been homed, current position is: \t%8.0f \n", theNode.Motion.PosnMeasured.Value());
    } else {
    printf("Node has not had homing setup through ClearView. The node will not be homed.\n");
    msgUser("Press any key to continue.");
    return 0;}

    // Homing program finished successfully
    return 1;
}


/**
 * @brief position control, single-tone testing program, generate a data file
 * @param[in]theNode: the shortcut of Node
 * @param[in]myMgr:   the pointer to the system reference
 * @retval -
 */
void pos_control(sFnd::INode& theNode, sFnd::SysManager* myMgr){
    // time variables for the test process
    double test_time = 60000;
    double start_time = myMgr->TimeStampMsec();
    double end_time = start_time + test_time;

    // generate the file name with time
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    char dateBuffer[80];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", localTime);
    string fileName = "data_" + string(dateBuffer) + ".txt";

    std::ofstream myfile(fileName);
    while(myMgr->TimeStampMsec() < end_time){
        int ab_position = 3200*cos(2*M_PI*(myMgr->TimeStampMsec() - start_time)/20000);
        theNode.Motion.MovePosnStart(ab_position,true);
        while(!theNode.Motion.MoveIsDone()){};
        theNode.Motion.PosnMeasured.Refresh(); // refresh the position value
        string data =to_string(theNode.Motion.PosnMeasured.Value()) + " " + to_string(myMgr->TimeStampMsec() -start_time) + "\n";
        myfile<< data;
    }
    myfile.close();
}


/**
 * @brief velocity control, single-tone testing program, generate a data file
 * @param[in]theNode: the shortcut of Node
 * @param[in]myMgr:   the pointer to the system reference
 * @retval -
 */
void vel_control(sFnd::INode& theNode, sFnd::SysManager* myMgr, int time_input, int amp, int period)
{
    // time variables for the test process
    double test_time = (double)time_input * 1000;
    double start_time = myMgr->TimeStampMsec();
    double end_time = start_time + test_time;

    // generate the file name with time
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    char dateBuffer[80];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d_%H-%M-%S", localTime);
    string fileName = "data_" + string(dateBuffer) + ".txt";

    ofstream myfile(fileName);
    while(myMgr->TimeStampMsec() < end_time){
        double present_vel = (double)amp / 1600 * 120 * M_PI / period * sin(2 * M_PI * (myMgr->TimeStampMsec() - start_time)/(double)(period * 1000));
        theNode.Motion.MoveVelStart(present_vel);
        while(!theNode.Motion.VelocityReachedTarget());
        theNode.Motion.PosnMeasured.Refresh(); // refresh the velocity value
        string data =to_string(theNode.Motion.PosnMeasured.Value()) + " " + to_string(myMgr->TimeStampMsec() -start_time) + "\n";
        myfile<< data;
    }
    myfile.close();
}


/**
 * @brief Multiple tones motion control through velocity start, generate the data file from the encoder
 * @param[in]theNode: the shortcut of the node
 * @param[in]myMgr:	 the pointer to the system reference
 * @param[in]len:	 the length of the input data set (wave amplitude and wave period)
 * @param[in]time_input: test time
 * @param[in]amp:	 the pointer to the array contains wave heights
 * @param[in]period:  the pointer to the array contains wave periods
 * @retval
 */
void multi_tone(sFnd::INode& theNode, sFnd::SysManager* myMgr, int len, int time_input, int *amp, int *period)
{
    // time variables for the test process
    double test_time = (double)time_input * 1000;
    double start_time = myMgr->TimeStampMsec();
    double end_time = start_time + test_time;

    // generate the file name with time
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    char dateBuffer[80];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d_%H-%M-%S", localTime);
    string fileName = "data_" + string(dateBuffer) + ".csv";

    ofstream myfile(fileName);
    myfile<< "TimeStamp(msec),TargetVelocity(rpm),CurrentVelocity(rpm),Torque(%),Position(cnt) \n";
    while(myMgr->TimeStampMsec() < end_time){
        double present_vel = 0;
        for (int index = 0; index < len; index++){
            if (amp[index] <= 0) {continue;}
            present_vel += (double)amp[index] / 1600 * 120 * M_PI / period[index] * sin(2 * M_PI * (myMgr->TimeStampMsec() - start_time)/(double)(period[index] * 1000));
        }
        theNode.Motion.MoveVelStart(present_vel);
        while(!theNode.Motion.VelocityReachedTarget());
		theNode.Motion.VelMeasured.Refresh();  // refresh the velocity value
        theNode.Motion.PosnMeasured.Refresh(); // refresh the position value
		theNode.Motion.TrqMeasured.Refresh();  // refresh the torque value
        string data =to_string(myMgr->TimeStampMsec() -start_time) + "," + to_string(present_vel) + "," + to_string(theNode.Motion.VelMeasured.Value()) + "," + to_string(theNode.Motion.TrqMeasured.Value()) + "," + to_string(theNode.Motion.PosnMeasured.Value()) + "\n";
        myfile<< data;
    }
    myfile.close();
}


/**
 * @brief           
 * @param[in/out/in,out]theNode:
 * @param[in/out/in,out]myMgr:
 * @param[in/out/in,out]time_input:
 * @param[in/out/in,out]fetch_distance:
 * @param[in/out/in,out]wind_speed:
 * @retval         
 */
void Jonswap_tone(sFnd::INode& theNode, sFnd::SysManager* myMgr, int time_input, int fetch_distance, float wind_speed)
{   
    // Generate Jonswap data set
    Jonswap myJonswap(fetch_distance, wind_speed, time_input, 0.5, 1.9);
    const std::vector<float>& time = myJonswap.getTIME();
    const std::vector<float>& speed = myJonswap.getSPEED();
    const std::vector<float>& waveheight = myJonswap.getETA();
    
    // Move to start point 
    theNode.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(theNode, 50, 50, 100);
    int init_pos_cnt = 10000 + int(M_TO_CNT(waveheight[0]));
    theNode.Motion.MovePosnStart(init_pos_cnt,true);
    while(!theNode.Motion.MoveIsDone()){};
    myMgr->Delay(2000); // Wait for two seconds at the start point

    // Start the Jonswap simulation
    theNode.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(theNode, 10000, 10000, 700); // re-config the motor
    for(size_t i = 0; i < time.size(); i++){
        double preset_vel = M_PER_SEC_TO_RPM(speed[i]);
        theNode.Motion.MoveVelStart(preset_vel);
        while(!theNode.Motion.VelocityReachedTarget()){};
    }
}



void Jonswap_tone_CML(sFnd::INode& theNode, sFnd::SysManager* myMgr)
{
    // Ask For Input
    int testtime = 0;
    // Ask the user for input and validate within the desired range
    while (true) {
        std::cout << "Enter the test time in seconds: ";
        if (std::cin >> testtime && testtime > 0) {
            break; // Valid input within the desired range
        } else {
            std::cout << "Invalid input. Please enter a valid time." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        }
    }

    int fetch = 0;
    // Ask the user for input and validate within the desired range
    while (true) {
        std::cout << "Enter the test fetch distance in KM: ";
        if (std::cin >> fetch && fetch > 0 && fetch <= 30){
            break; // Valid input within the desired range
        } else {
            std::cout << "Invalid input. Please enter a valid fetch distance between 0 and 30 KM." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        }
    }

    float U_10 = 0;
    // Ask the user for input and validate within the desired range
    while (true) {
        std::cout << "Enter the test wind speed in knots: ";
        if (std::cin >> U_10 && U_10 > 0){
            break; // Valid input within the desired range
        } else {
            std::cout << "Invalid input. Please enter a valid period larger than 0 second." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        }
    }

    Jonswap_tone(theNode, myMgr, testtime, fetch, U_10);
}


/**
 * @brief Single tone motion control base on velocity control, ask for user input
 * @param[in]theNode: the short cut of the Node
 * @param[in]myMgr: the pointer to the system reference
 * @retval
 */
void SingleTone(sFnd::INode& theNode, sFnd::SysManager* myMgr)
{
    // Ask For Input
    int testtime = 0;
    // Ask the user for input and validate within the desired range
    while (true) {
        std::cout << "Enter the test time in seconds: ";
        if (std::cin >> testtime && testtime > 0) {
            break; // Valid input within the desired range
        } else {
            std::cout << "Invalid input. Please enter a valid time." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        }
    }

    int amp = 0;
    // Ask the user for input and validate within the desired range
    while (true) {
        std::cout << "Enter the test amplitude in counts: ";
        if (std::cin >> amp && amp > 0 && amp <= 10000){
            break; // Valid input within the desired range
        } else {
            std::cout << "Invalid input. Please enter a valid amplitude between 0 and 10000 counts." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        }
    }

    int period = 0;
    // Ask the user for input and validate within the desired range
    while (true) {
        std::cout << "Enter the test wave period in seconds: ";
        if (std::cin >> period && period > 0){
            break; // Valid input within the desired range
        } else {
            std::cout << "Invalid input. Please enter a valid period larger than 5 seconds." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        }
    }


    // Basic motion control
    theNode.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(theNode, 10000, 10000, 700); // Set the node configuration
    // std::thread rock(CartMove, testtime);
    vel_control(theNode, myMgr, testtime, amp, period);
    // rock.join();
}


/**
 * @brief Multi-tone motion control based on velocity control, ask for user input
 * @param[in]theNode: the short cut of the node
 * @param[in]myMgr: the pointer to the system reference
 * @retval
 */
void MultiTone(sFnd::INode& theNode, sFnd::SysManager* myMgr)
{
    // Ask For Input
    int testtime = 0;
    // Ask the user for input and validate within the desired range
    while (true) {
        std::cout << "Enter the test time in seconds: ";
        if (std::cin >> testtime && testtime > 0) {
            break; // Valid input within the desired range
        } else {
            std::cout << "Invalid input. Please enter a valid time." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        }
    }

    int amp[10] = {0};
    int period[10] = {0};
    int index = 0;
    int sum = 0;
    bool input_flag = true;
    while (input_flag) {
        bool amp_flag = false;
        bool period_flag = false;
        while (true) {
            // Amplitude input
            std::cout << "Enter the test amplitude in counts: ";
            if (std::cin >> amp[index] && amp[index] > 0 && amp[index] <= 10000){
                amp_flag = true;
                sum += amp[index];
                break; // Valid input within the desired range
            } else {
                std::cout << "Invalid input. Please enter a valid amplitude between 0 and 2000 counts." << std::endl;
                std::cin.clear(); // Clear error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
            }
        }

        while (true) {
            // Period input
            std::cout << "Enter the corresponding wave period in seconds ";
            if (std::cin >> period[index] && period[index] > 0){
                period_flag = true;
                break; // Valid input within the desired range
            }else {
                std::cout << "Invalid input. Please enter a valid amplitude between 0 and 2000 counts." << std::endl;
                std::cin.clear(); // Clear error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
            }
        }

        if (amp_flag && period_flag){index++;}
        else {amp[index] = 0; period[index] = 0;};

        // Ask for continuous input
        bool continuous = false;
        char select;
        while (true) {
            // Cotinuous input
            std::cout << "Already have " << index << " sets of valid data." << std::endl;
            std::cout << "Do you want to add another wave tone? [Y/N]";
            std::cin >> select;
            if (select == 'Y' || select == 'y'){continuous = true; break;}
            else if (select == 'N' || select == 'n'){continuous = false; break;}
            else {std::cout << "Invalid input. Please enter Y or N" << std::endl;};
        }

        // Check if get enough valid input
        if (continuous) {
            if (index < 10 && sum <= 10000){input_flag = true;}
            else if (index = 10){std::cout << "Already ten sets input, done!" << std::endl; input_flag = false;}
            else if (sum > 10000){std::cout << "Already larger than 10000 cnts, delete the last tone!" << std::endl; amp[index] = 0; period[index] = 0; input_flag = false;}}
        else if (!continuous) {
            if (index && (index <= 10)){input_flag = false;}
            else if (index == 0) {std::cout << "No invalid input, please keep entering!" << std::endl; input_flag = true;}
        }
    }

    // Multiple tone motion control
    theNode.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(theNode, 10000, 10000, 700); // Set the node configuration
    // std::thread rock(CartMove, testtime);
    multi_tone(theNode, myMgr, index, testtime, amp, period);
    // rock.join();
}
