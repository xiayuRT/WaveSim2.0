/******************************************************************************/
/** 
	Using Communications Hub for General Purpose Outputs, and Inputs

	This example demonstrates setting the Brake Control mode to use a
	communication hub Brake output for general purpose usages.  This example
	also reads the local General purpose inputs at the Nodes.
**/
/******************************************************************************/

#include <stdio.h>
#include <string>
#include <thread>
#include <iostream>
#include <cmath>
#include "inc-pub/pubSysCls.h"
#include "Serial/serial.h"

using namespace sFnd;
using namespace std;
/********************************* Macros **************************************/
#define TIME_TILL_TIMEOUT 10000; // timeout used for status waiting 

/********************************* Functions ***********************************/
void msgUser(const char *msg);
int port_init(void);
void node_info(INode& theNode);

/****************************** Global Variables *******************************/
size_t portCount = 0;
std::vector<std::string> comHubPorts;
SysManager* myMgr = SysManager::Instance();							//Create System Manager myMgr

int main(int argc, char* argv[])
{	
	/***************** Pi & Arduino Test Code *****************/
	/*
	// Serial initilaize
	serial::Serial my_serial("/dev/ttyACM0", 9600, serial::Timeout::simpleTimeout(3000));
	my_serial.close();
	std::cout << "Serial closed" << std::endl;
	my_serial.open();
	while(!my_serial.isOpen()){;}
	std::cout << "Serial opened" << std::endl;
	
	int amp(0);
    std::cout << "Enter the angle:" << std::endl;
    std::cin >> amp;
    std::string amp_str = std::to_string(amp);
    
    // Send to arduino
    std::string test_str = "Start amp " + amp_str + "\n";
    size_t bytesWritten = my_serial.write(test_str);
    my_serial.flushOutput();
    my_serial.close();
    std::this_thread::sleep_for(std::chrono::seconds(2));
	*/
    
	msgUser("Base motor single-tone testing is starting. Press Enter to continue.");

	//This will try to open the port. If there is an error/exception during the port opening,
	//the code will jump to the catch loop where detailed information regarding the error will be displayed;
	//otherwise the catch loop is skipped over
	try
	{	
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Initialization
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if (!port_init()){return -1;} // if unable to open the port, terminate the main program

		// Once the code gets past this point, it can be assumed that the port has been opened without issue
		// Then get a reference the port object
		IPort &myPort = myMgr->Ports(0);
		
		// Print the state of the port
		printf("Port[%d]: state=%d, nodes=%d\n", myPort.NetNumber(), myPort.OpenState(), myPort.NodeCount());

		/*To-do: get detailed node initialization checking */
		if (!myPort.NodeCount()){return -1;} // terminate the program if there is no Node connection
		
		// Create a shortcut reference for a node
		INode &theNode = myPort.Nodes(0); // there supposed to be only one node 

		theNode.EnableReq(false); // ensure Node is disabled before loading config file

		myMgr->Delay(200);

		/* To-do: generate config file and try to load it from the path */
		/* To-do: develop homing config */
		theNode.Setup.ConfigLoad("/home/wave/Desktop/Base-Motor/Motor_config/base_test.mtr"); 

		/* To-do: develop function to get info of the node */
		// Print the state of the Node
		node_info(theNode);

		// Following states will attempt to enable the node
		// First, any shutdowns or NodeStops are cleared, finally the node is enabled
		theNode.Status.AlertsClear();
		theNode.Motion.NodeStopClear();
		theNode.EnableReq(true);
		printf("Node is enabled.\n");
		double timeout = myMgr->TimeStampMsec() + TIME_TILL_TIMEOUT; // define a timeout in case the node is unable to move
		while (!theNode.Motion.IsReady()) {
			if (myMgr->TimeStampMsec() > timeout) {
				printf("Error: Timed out waiting for Node to enable\n");
				msgUser("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key
				return -2;
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Start Homing Program
		///////////////////////////////////////////////////////////////////////////////////////////////////// 
		if (theNode.Motion.Homing.HomingValid()){
			if (theNode.Motion.Homing.WasHomed()){
				printf("Node has already been homed, current position is: \t%8.0f \n", theNode.Motion.PosnMeasured.Value());
				printf("Rehoming Node... \n");
			}
			else 
			{
				printf("Node has not been homed. Homing Node now...\n");
			}

			// Home the Node
			theNode.Motion.Homing.Initiate();
			timeout = myMgr->TimeStampMsec() + TIME_TILL_TIMEOUT;
			while (!theNode.Motion.Homing.WasHomed()){
				if(myMgr->TimeStampMsec() > timeout){
					printf("Node did not completing homing: \n\t -Ensure Homing settings have been defined through ClearView. \n\t -Check for alerts/Shutdowns \n\t -Ensure timeout is longer than the longest possible homing move.\n");
					msgUser("Press any key to continue.");
					return -2;
				}
			}
			printf("Node completed homing\n");
			printf("Current position is: \t%0.8f \n", theNode.Motion.PosnMeasured.Value());
		} else {
			printf("Node has not had homing setup through ClearView. The node will not be homed.\n");
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Basic motion control
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		theNode.Motion.MoveWentDone(); // Clear the rising edge move done register
		theNode.AccUnit(INode::RPM_PER_SEC); // Set the units for Acceleration to RPM/SEC
		theNode.VelUnit(INode::RPM); // Set the units for Velocity to RPM
		theNode.Motion.AccLimit = 10000;
		theNode.Motion.VelLimit = 500;

		double test_time = 60000;
		double start_time = myMgr->TimeStampMsec();
		double end_time = start_time + test_time;

		while(myMgr->TimeStampMsec() < end_time){
			int ab_position = 3200*cos(2*M_PI*(myMgr->TimeStampMsec() - start_time)/20000);
			theNode.Motion.MovePosnStart(ab_position,true);
		} 

		printf("End of test!\n");
		theNode.Motion.PosnMeasured.Refresh(); // refresh the position value
		printf("Current position is: \t%8.0f \n", theNode.Motion.PosnMeasured.Value());
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// Close the node and port after operation
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		printf("Disabling nodes, and closing port\n");
		theNode.EnableReq(false);
		myMgr->PortsClose(); 
	}
	catch (mnErr& theErr) {
		fprintf(stderr, "Caught error: addr=%d, err=0x%0x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		printf("Caught error: addr=%d, err=0x%08x\nmsg=%s\n",
				theErr.TheAddr, theErr.ErrorCode, theErr.ErrorMsg);
		msgUser("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key
		// keep the console output readable
		printf("\n");
		return(2);
	}
	catch (...) {
		fprintf(stderr, "Error generic caught\n");
		printf("Generic error caught\n");
		msgUser("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key
		// keep the console output readable
		printf("\n");
		return(3);
	}

	// Good-bye
	msgUser("Press any key to continue."); //pause so the user can see the error message; waits for user to press a key
	// keep the console output readable
	printf("\n");
	return 0;
}
//																			   *
//******************************************************************************


/********************************** Functions **********************************/

// Send message and wait for newline
void msgUser(const char *msg) {
	std::cout << msg;
	getchar();
}

// Initilaize the port, return 1 if success, return 0 if falied
int port_init(void){

	SysManager::FindComHubPorts(comHubPorts);
	printf("Found %d SC Hubs\n", comHubPorts.size());

	for (portCount = 0; portCount < comHubPorts.size() && portCount < NET_CONTROLLER_MAX; portCount++) {
		myMgr->ComHubPort(portCount, comHubPorts[portCount].c_str()); 	//define the first SC Hub port (port 0) to be associated 
										// with COM portnum (as seen in device manager)
	}

	// Only one port used in wave sim system
	if (portCount == 1) {
		//printf("\n I will now open port \t%i \n \n", portnum);
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

// Print the node's information
void node_info(INode& theNode){
	printf("           Node[0]: type=%d\n", theNode.Info.NodeType());
	printf("            userID: %s\n", theNode.Info.UserID.Value());
	printf("        FW version: %s\n", theNode.Info.FirmwareVersion.Value());
	printf("          Serial #: %d\n", theNode.Info.SerialNumber.Value());
	printf("             Model: %s\n", theNode.Info.Model.Value());
}
