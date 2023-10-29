/**
 * ****************************(C) COPYRIGHT 2023 Blue Bear****************************
 * @file       main.cpp
 * @brief      the thread integrate with both the base motor's motion and 
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

#include <stdio.h>
#include <iostream>
#include <thread>
#include <limits>

#include "inc-pub/pubSysCls.h"
#include "basemotor.hpp"
#include "cartthread.hpp"

/********************************* Name Space **************************************/
using namespace sFnd;
using namespace std;

/********************************* Macros ******************************************/
#define TIME_TILL_TIMEOUT 10000; // timeout used for status waiting 

/********************************* Global ******************************************/
SysManager* myMgr = SysManager::Instance();							//Create System Manager myMgr pointer

/******************************** Function ******************************************/
int main()
{	
	msgUser("WaveSim2.0 test is starting. Press Enter to continue.");

	try
	{	
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Initialization
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// port initialization
		if (!port_init(myMgr)){return -1;} // if unable to open the port, terminate the main program
		IPort &myPort = myMgr->Ports(0);   // create a reference to the port
		port_info(myPort);                 // print the information of the port

		// node initialization
		if (!myPort.NodeCount()){return -1;} // terminate the program if there is no Node connection
		INode &theNode = myPort.Nodes(0);    // create a shortcut reference for a node
		// load configuration (homing program, g-stop)
		node_load_config(theNode, myMgr, "/home/wave/Desktop/subscale_test.mtr");
		node_info(theNode);					 // Print the state of the Node
		node_enable(theNode);  // Following states will attempt to enable the node


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Motion Start - Homing
		///////////////////////////////////////////////////////////////////////////////////////////////////// 
		if (!homing(theNode, myMgr)) {
			std::cout << "Homing failed, terminate the program." << std::endl;
			msgUser("Press any key to continue.");
			std::cout << "\n" << std::endl;
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Basic motion control
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		while(true){
			// Ask for user input
			int mode = 0;
			std::cout<< "Select the testing mode:" << std::endl;
			std::cout<< "[1] Single/Multiple tone test." << std::endl;
			std::cout<< "[2] Jonswap mode tone test." << std::endl;
			while (true) {
				std::cout << "Enter the mode: ";
				if (std::cin >> mode && (mode == 1 || mode == 2)) {
					break; // Valid input within the desired range
				} else {
					std::cout << "Invalid input. Please enter a valid mode." << std::endl;
					std::cin.clear(); // Clear error flags
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
				}
			}
			switch(mode){
				case(1): Multi_tone_CML(theNode, myMgr); break;
				case(2): Jonswap_tone_CML(theNode, myMgr); break;
				default: std::cout << "Invalid input mode, select mode again." << std::endl;
			}


			// Motion End - Homing
			if (!homing(theNode, myMgr)) {
				std::cout << "Homing failed, terminate the program." << std::endl;
				msgUser("Press any key to continue.");
				std::cout << "\n" << std::endl;
			}

			// Ask for another test
			char select = 'N';
			do{
				std::cout << "Do you want to test again? [Y/N]: ";
				std::cin >> select;

				if (select == 'Y' || select == 'y'){
				} else if (select == 'N' || select == 'n'){
					break;
				} else {
					std::cout << "Invalid input. Please enter Y or N." << std::endl;
				}
			}while(select != 'Y' && select != 'y');
			if (select == 'N' || select == 'n'){ break; }
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Close the node after operation
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		printf("End of test!\n Disabling nodes, and closing port\n");
		theNode.EnableReq(false);
		// Close the port
		myMgr->PortsClose(); 
		msgUser("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key // keep the console output readable
		printf("\n");
		return 0;

	}

	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		msgUser("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key // keep the console output readable
		printf("\n");
		return(2);
	}

	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		msgUser("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key	// keep the console output readable
		printf("\n");
		return(3);
	}

}


