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
#include "inc-pub/pubSysCls.h"
#include "Serial/serial.h"

using namespace sFnd;
using namespace std;

/********************************* Functions ***********************************/
void msgUser(const char *msg);
int port_init(void);

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
    
	std::cout<< "GPIO Example starting. Press Enter to continue." << std::endl;
	msgUser("GPIO Example starting. Press Enter to continue.");

	//This will try to open the port. If there is an error/exception during the port opening,
	//the code will jump to the catch loop where detailed information regarding the error will be displayed;
	//otherwise the catch loop is skipped over
	try
	{	
		if (!port_init){return -1;} // if unable to open the port, terminate the main program

		// Once the code gets past this point, it can be assumed that the port has been opened without issue
		// Then get a reference the port object
		IPort &myPort = myMgr->Ports(0);
		
		// Print the state of the port
		printf(" Port[%d]: state=%d, nodes=%d\n", myPort.NetNumber(), myPort.OpenState(), myPort.NodeCount());

		// To-do get detailed node initialization checking 
		if (!myPort.NodeCount()){return -1;} // terminate the program if there is no Node connection
		// Create a shortcut reference for a node
		INode &myNode = myPort.Nodes(0); 

	
		// To-do close the port after operation
		// myMgr->PortsClose(); // Close down the ports
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
