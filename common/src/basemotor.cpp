/**
 * **************************(C) COPYRIGHT 2023 Running Tide**************************
 * @file       basemotor.cpp
 * @brief      
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
 * **************************(C) COPYRIGHT 2023 Running Tide**************************
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>
#include "../../external/inc-pub/pubSysCls.h"

#include <fstream>
#include <ctime>
#include <cmath>
#include <limits>

#include "../inc/basemotor.hpp"
#include "../inc/cartthread.hpp"
#include "../inc/Jonswap.hpp"

using namespace sFnd;
using namespace std;


#define JONSWAP_HEIGHT_LIMIT 0.95
#define HIGH_CUT_FREQ 0.5
#define HOME_POS 0
#define M_TO_CM 100
#define KM_TO_M 1000
#define CNT_PER_ROUND 800
#define PULLEY_DIAMETER_CM 4.85
#define HZ_TO_MSCE(freq) (1000 / freq)
#define M_PER_SEC_TO_RPM(vel) ((vel) * 60 * M_TO_CM / (M_PI * PULLEY_DIAMETER_CM)) 
#define M_TO_CNT(len) ((len) * M_TO_CM * CNT_PER_ROUND / (M_PI * PULLEY_DIAMETER_CM))
#define CNT_TO_CM(step) ((step) * M_PI * PULLEY_DIAMETER_CM / CNT_PER_ROUND)


void msg_user(const char *msg) {
    cout << msg;
    getchar();
}


int port_init(sFnd::SysManager* my_mgr){
    size_t portCount = 0;
    vector<std::string> comHubPorts;
    SysManager::FindComHubPorts(comHubPorts);
    printf("Found %d SC Hubs\n", comHubPorts.size());

    for (portCount = 0; portCount < comHubPorts.size() && portCount < NET_CONTROLLER_MAX; portCount++) {
        my_mgr->ComHubPort(portCount, comHubPorts[portCount].c_str()); 	//define the first SC Hub port (port 0) to be associated
                                        // with COM portnum (as seen in device manager)
    }

    // Only one port used in wave sim system
    if (portCount == 1) {
        my_mgr->PortsOpen(portCount);				//Open the port
        cout << "Port is opened successfully!" << endl;
        return 1;
    } else if (portCount > 1){
        cout << "Port num is larger than 1, check SC4 HUB carefully!" << endl;
        msg_user("Press any key to continue."); // pause so the user can see the error message; waits for user to press a key
        return 0;
    } else {
        printf("Unable to locate SC hub port\n");
        msg_user("Press any key to continue."); // pause so the user can see the error message; waits for user to press a key
        return 0;  // This returns error
    }
}


void node_info(sFnd::INode& theNode){
    printf("Node[0]: type=%d\n", theNode.Info.NodeType());
    printf("userID: %s\n", theNode.Info.UserID.Value());
    printf("FW version: %s\n", theNode.Info.FirmwareVersion.Value());
    printf("Serial #: %d\n", theNode.Info.SerialNumber.Value());
    printf("Model: %s\n", theNode.Info.Model.Value());
}


void port_info(sFnd::IPort& thePort){
    printf("Port[%d]: state=%d, nodes=%d\n", thePort.NetNumber(), thePort.OpenState(), thePort.NodeCount());
}


void node_enable(sFnd::INode& theNode){
    theNode.Status.AlertsClear();   // clear shutdowns
    theNode.Motion.NodeStopClear(); // clear NodeStops
    theNode.EnableReq(true);        // enable the Node
    printf("Node is enabled.\n");
}


void node_config(sFnd::INode& theNode, int Acc_limit, int Dec_limit, int Vel_limit){
    theNode.AccUnit(INode::RPM_PER_SEC);       // Set the units for Acceleration to RPM/SEC
    theNode.VelUnit(INode::RPM);               // Set the units for Velocity to RPM
    theNode.Motion.AccLimit = Acc_limit;       // Set the limit of Acceleration
    theNode.Motion.Adv.DecelLimit = Dec_limit; // Set the limit of Deceleration
    theNode.Motion.VelLimit = Vel_limit;       // Set the limit of Velocity
}


void node_load_config(sFnd::INode& theNode, sFnd::SysManager* my_mgr, const char * address){
    theNode.EnableReq(false);          // ensure Node is disabled before loading config file
    my_mgr->Delay(200);
    theNode.Setup.ConfigLoad(address); // load configuration
}


int homing(sFnd::INode& theNode, sFnd::SysManager* my_mgr){
    double TIME_TILL_TIMEOUT = 80000; // timeout used for status waiting
    double timeout = my_mgr->TimeStampMsec() + TIME_TILL_TIMEOUT; // define a timeout in case the node is unable to move
    while (!theNode.Motion.IsReady()) {
        if (my_mgr->TimeStampMsec() > timeout) {
            printf("Error: Timed out waiting for Node to enable\n");
            msg_user("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key
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
        timeout = my_mgr->TimeStampMsec() + TIME_TILL_TIMEOUT;
        while (!theNode.Motion.Homing.WasHomed()){
            if(my_mgr->TimeStampMsec() > timeout){
                printf("Node did not completing homing: \n\t -Ensure Homing settings have been defined through ClearView. \n\t -Check for alerts/Shutdowns \n\t -Ensure timeout is longer than the longest possible homing move.\n");
                msg_user("Press any key to continue.");
                return 0;
            }
        }
        printf("Node completed homing\n");
        printf("Node has already been homed, current position is: \t%8.0f \n", theNode.Motion.PosnMeasured.Value());

    } else {
    printf("Node has not had homing setup through ClearView. The node will not be homed.\n");
    msg_user("Press any key to continue.");
    return 0;}

    // Homing program finished successfully
    return 1;
}


void pos_control(sFnd::INode& theNode, sFnd::SysManager* my_mgr){
    // time variables for the test process
    double test_time = 60000;
    double start_time = my_mgr->TimeStampMsec();
    double end_time = start_time + test_time;

    // generate the file name with time
    time_t current_time = time(nullptr);
    tm* local_time = localtime(&current_time);
    char date_buffer[80];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", local_time);
    string file_name = "data_" + string(date_buffer) + ".txt";

    std::ofstream myfile(file_name);
    while(my_mgr->TimeStampMsec() < end_time){
        int ab_position = 3200*cos(2*M_PI*(my_mgr->TimeStampMsec() - start_time)/20000);
        theNode.Motion.MovePosnStart(ab_position,true);
        while(!theNode.Motion.MoveIsDone()){};
        theNode.Motion.PosnMeasured.Refresh(); // refresh the position value
        string data =to_string(theNode.Motion.PosnMeasured.Value()) + " " + to_string(my_mgr->TimeStampMsec() -start_time) + "\n";
        myfile<< data;
    }
    myfile.close();
}


void vel_control(sFnd::INode& theNode, sFnd::SysManager* my_mgr, int time_input, int amp, int period)
{
    // time variables for the test process
    double test_time = (double)time_input * 1000;
    double start_time = my_mgr->TimeStampMsec();
    double end_time = start_time + test_time;

    // generate the file name with time
    time_t current_time = time(nullptr);
    tm* local_time = localtime(&current_time);
    char date_buffer[80];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d_%H-%M-%S", local_time);
    string file_name = "data_" + string(date_buffer) + ".txt";

    ofstream myfile(file_name);
    while(my_mgr->TimeStampMsec() < end_time){
        double present_vel = 5 * (double)amp / 1600 * 120 * M_PI / period * sin(2 * M_PI * (my_mgr->TimeStampMsec() - start_time)/(double)(period * 1000));
        theNode.Motion.MoveVelStart(present_vel);
        while(!theNode.Motion.VelocityReachedTarget());
        theNode.Motion.PosnMeasured.Refresh(); // refresh the velocity value
        string data =to_string(theNode.Motion.PosnMeasured.Value()) + " " + to_string(my_mgr->TimeStampMsec() -start_time) + "\n";
        myfile<< data;
    }
    myfile.close();
}


void multi_tone(sFnd::INode& theNode, sFnd::SysManager* my_mgr, int len, int time_input, int *amp, int *period, bool rock)
{
    // time variables for the test process
    double test_time = (double)time_input * 1000;
    double start_time = my_mgr->TimeStampMsec();
    double end_time = start_time + test_time;

    // generate the file name with time
    time_t current_time = time(nullptr);
    tm* local_time = localtime(&current_time);
    char date_buffer[80];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d_%H-%M-%S", local_time);
    string file_name = "data_" + string(date_buffer) + ".csv";
    ofstream myfile(file_name);
    myfile<< "TimeStamp(msec),TargetVelocity(rpm),CurrentVelocity(rpm),Torque(%),Position(cnt) \n";
    myfile<< "Single / Multiple Tones Test \n";
    if(rock){myfile << "Rocking is on.\n";}
    for (int index = 0; index < len; index++){
        string data = "Tone" + to_string(index) + ":," + to_string(amp[index]) + ",cnts," + to_string(period[index]) + ",seconds \n" ;
        myfile<<data;
    }

    // start simulation
    if(rock){
        std::thread rocking(cart_move, time_input);
        while(my_mgr->TimeStampMsec() < end_time){
            double present_vel = 0;
            for (int index = 0; index < len; index++){
                if (amp[index] <= 0) {continue;}
                present_vel += (double)amp[index] / 1600 * 120 * M_PI / period[index] * sin(2 * M_PI * (my_mgr->TimeStampMsec() - start_time)/(double)(period[index] * 1000));
            }
            theNode.Motion.MoveVelStart(present_vel);
            while(!theNode.Motion.VelocityReachedTarget());
            theNode.Motion.VelMeasured.Refresh();  // refresh the velocity value
            theNode.Motion.PosnMeasured.Refresh(); // refresh the position value
            theNode.Motion.TrqMeasured.Refresh();  // refresh the torque value
            string data =to_string(my_mgr->TimeStampMsec() -start_time) + "," + to_string(present_vel) + "," + to_string(theNode.Motion.VelMeasured.Value()) + "," + to_string(theNode.Motion.TrqMeasured.Value()) + "," + to_string(theNode.Motion.PosnMeasured.Value()) + "\n";
            myfile<< data;
        }
        rocking.join();
    }else{
        while(my_mgr->TimeStampMsec() < end_time){
            double present_vel = 0;
            for (int index = 0; index < len; index++){
                if (amp[index] <= 0) {continue;}
                present_vel += (double)amp[index] / 1600 * 120 * M_PI / period[index] * sin(2 * M_PI * (my_mgr->TimeStampMsec() - start_time)/(double)(period[index] * 1000));
            }
            theNode.Motion.MoveVelStart(present_vel);
            while(!theNode.Motion.VelocityReachedTarget());
            theNode.Motion.VelMeasured.Refresh();  // refresh the velocity value
            theNode.Motion.PosnMeasured.Refresh(); // refresh the position value
            theNode.Motion.TrqMeasured.Refresh();  // refresh the torque value
            string data =to_string(my_mgr->TimeStampMsec() -start_time) + "," + to_string(present_vel) + "," + to_string(theNode.Motion.VelMeasured.Value()) + "," + to_string(theNode.Motion.TrqMeasured.Value()) + "," + to_string(theNode.Motion.PosnMeasured.Value()) + "\n";
            myfile<< data;
        }
    }

    myfile.close();
}


void Jonswap_tone(sFnd::INode& theNode, sFnd::SysManager* my_mgr, int time_input, int fetch_distance, float wind_speed, bool rock)
{   
    // Generate Jonswap data set
    Jonswap jonswap(fetch_distance, wind_speed, time_input, 0.5);
    if(!jonswap.cal_limited_eta(time_input,HIGH_CUT_FREQ, JONSWAP_HEIGHT_LIMIT)){
        std::cout << "Can not generate suitable displacement set. Exit Jonswap mode!" << std::endl;
        return;};

    //const std::vector<float>& time_series = jonswap.get_time();
    const std::vector<float>& speed = jonswap.get_speed();
    const std::vector<float>& waveheight = jonswap.get_eta();

    // generate the file name with time
    time_t current_time = time(nullptr);
    tm* local_time = localtime(&current_time);
    char date_buffer[80];
    strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d_%H-%M-%S", local_time);
    string file_name = "data_" + string(date_buffer) + ".csv";
    ofstream myfile(file_name);
    myfile<< "TimeStamp(msec),TargetVelocity(rpm),CurrentVelocity(rpm),Torque(%),TargetPosition(cnt),Position(cnt) \n";
    myfile<< "Jonswap Test \n";
    if(rock){myfile << "Rocking is on.\n";}
    string data = "Fetch Distance(km):" + to_string(fetch_distance/KM_TO_M) + "Wind Speed U_10(knots):" + to_string(wind_speed) + "\n";

    // Move to start point 
    theNode.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(theNode, 50, 50, 100);
    int init_pos_cnt = int(5000 + M_TO_CNT(waveheight[0]));
    std::cout << "Init position: " << init_pos_cnt << std::endl;
    theNode.Motion.MovePosnStart(init_pos_cnt,true);
    while(!theNode.Motion.MoveIsDone());
    std::cout << "Wait for one second to start... " << std::endl; 
    my_mgr->Delay(1000); // Wait for two seconds at the start point

    // Start the Jonswap simulation
    theNode.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(theNode, 10000, 10000, 700); // re-config the motor
    double start_time = my_mgr->TimeStampMsec();
    double start_point;
    double max_displacement_error_cnts = 0;

    // start simulation
    if(rock){
        std::thread rocking(cart_move, time_input);
        for(size_t i = 0; i < waveheight.size(); i++){
            // Velocity control
            theNode.Motion.MoveVelStart(M_PER_SEC_TO_RPM(speed[i]));
            // Cast each loop to 30hz
            if(!i){start_point = my_mgr->TimeStampMsec() - start_time;}
            else{while((my_mgr->TimeStampMsec() - start_time) < HZ_TO_MSCE(30) * i + start_point);} 
            // Data collection
            theNode.Motion.VelMeasured.Refresh();  // refresh the velocity value
            theNode.Motion.PosnMeasured.Refresh(); // refresh the position value
            theNode.Motion.TrqMeasured.Refresh();  // refresh the torque value
            double tmp_displacement_error_cnt = std::abs(theNode.Motion.PosnMeasured.Value() - 5000 - M_TO_CNT(waveheight[i]));
            string data =to_string(my_mgr->TimeStampMsec() - start_time) + "," + to_string(M_PER_SEC_TO_RPM(speed[i])) + "," + to_string(theNode.Motion.VelMeasured.Value()) + "," + to_string(theNode.Motion.TrqMeasured.Value()) + "," + to_string(M_TO_CNT(waveheight[i])) + "," + to_string(theNode.Motion.PosnMeasured.Value() - 5000) + "\n";
            myfile << data;
            if(tmp_displacement_error_cnt >= max_displacement_error_cnts){max_displacement_error_cnts = tmp_displacement_error_cnt;}
        }
        rocking.join();
        // Print out analyze result
        std::cout << "End of test.\n" << "The maximum displacement error: " << CNT_TO_CM(max_displacement_error_cnts) << "cm." << std::endl; 
    }else{
        for(size_t i = 0; i < waveheight.size(); i++){
            // Velocity control
            theNode.Motion.MoveVelStart(M_PER_SEC_TO_RPM(speed[i]));
            // Cast each loop to 30hz
            if(!i){start_point = my_mgr->TimeStampMsec() - start_time;}
            else{while((my_mgr->TimeStampMsec() - start_time) < HZ_TO_MSCE(30) * i + start_point);}
            // Data collection
            theNode.Motion.VelMeasured.Refresh();  // refresh the velocity value
            theNode.Motion.PosnMeasured.Refresh(); // refresh the position value
            theNode.Motion.TrqMeasured.Refresh();  // refresh the torque value
            double tmp_displacement_error_cnt = std::abs(theNode.Motion.PosnMeasured.Value() - 5000 - M_TO_CNT(waveheight[i]));
            //std::cout << tmp_displacement_error_cnt << std::endl;
            string data =to_string(my_mgr->TimeStampMsec() - start_time) + "," + to_string(M_PER_SEC_TO_RPM(speed[i])) + "," + to_string(theNode.Motion.VelMeasured.Value()) + "," + to_string(theNode.Motion.TrqMeasured.Value()) + "," + to_string(M_TO_CNT(waveheight[i])) + "," + to_string(theNode.Motion.PosnMeasured.Value() - 5000) + "\n";
            myfile << data;
            if(tmp_displacement_error_cnt >= max_displacement_error_cnts){max_displacement_error_cnts = tmp_displacement_error_cnt;}
        }
        // Print out analyze result
        std::cout << "End of test.\n" << "The maximum displacement error: " << CNT_TO_CM(max_displacement_error_cnts) << "cm." << std::endl;
    }
}


void Jonswap_tone_cml(sFnd::INode& theNode, sFnd::SysManager* my_mgr)
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

    char select;
    bool rock = false;
    // Ask the user for rocking input
    while(true) {
        std::cout << "Add rocking to this test? [Y/N]: ";
        std::cin >> select;
        if (select == 'Y' || select == 'y'){rock = true; break;}
        else if (select == 'N' ||select == 'n'){rock = false; break;} 
        else {std::cout << "Invalid input. Please enter Y or N." << std::endl;}
    }

    Jonswap_tone(theNode, my_mgr, testtime, fetch * KM_TO_M, U_10, rock);
}


void single_tone_cml(sFnd::INode& the_node, sFnd::SysManager* my_mgr)
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
    the_node.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(the_node, 10000, 10000, 700); // Set the node configuration
    vel_control(the_node, my_mgr, testtime, amp, period);
}


void multi_tone_cml(sFnd::INode& the_node, sFnd::SysManager* my_mgr)
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
            else if (index == 10){std::cout << "Already ten sets input, done!" << std::endl; input_flag = false;}
            else if (sum > 10000){std::cout << "Already larger than 10000 cnts, delete the last tone!" << std::endl; amp[index] = 0; period[index] = 0; input_flag = false;}}
        else if (!continuous) {
            if (index && (index <= 10)){input_flag = false;}
            else if (index == 0) {std::cout << "No invalid input, please keep entering!" << std::endl; input_flag = true;}
        }
    }

    char select;
    bool rock = false;
    // Ask the user for rocking input
    while(true) {
        std::cout << "Add rocking to this test? [Y/N]: ";
        std::cin >> select;
        if (select == 'Y' || select == 'y'){rock = true; break;}
        else if (select == 'N' ||select == 'n'){rock = false; break;} 
        else {std::cout << "Invalid input. Please enter Y or N." << std::endl;}
    }

    // Multiple tone motion control
    the_node.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(the_node, 10000, 10000, 700); // Set the node configuration
    multi_tone(the_node, my_mgr, index, testtime, amp, period, rock);

    // Move to zero (based on encoder output)
    the_node.Motion.MoveWentDone(); // Clear the rising edge move done register
    node_config(the_node, 50, 50, 100);
    my_mgr->Delay(2000); // Wait 2 second before rezero
    the_node.Motion.MovePosnStart(HOME_POS, true);
    while(!the_node.Motion.MoveIsDone()); 
    msg_user("Rezero done! Check the position.\n");
}
