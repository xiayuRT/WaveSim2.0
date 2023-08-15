#include "third-party/include/serial/serial.h"
#include <iostream>
#include "thread"
#include <string>

// Initialize the serial port with arduino uno
serial::Serial my_serial("COM4", 9600, serial::Timeout::simpleTimeout(3000));

// Functions
int SERIAL_PORT_CHECK();
void SEND_AMP();
void SEND_FRE();

// Main function
int main()
{
    while(!my_serial.isOpen()){};

    SEND_AMP();
    std::string response = my_serial.read(20);
    my_serial.flushInput();
    std::cout << response << std::endl;

    SEND_FRE();
    std::string response_r = my_serial.read(20);
    my_serial.flushInput();
    std::cout << response_r << std::endl;
}


/*
*@brief-> Check the state of serial port
*@param-> None
*@return->  1: "Port opened succesfully"
            0: "Port failed to open"
*/
int SERIAL_PORT_CHECK(){
    if (my_serial.isOpen())
    {
        std::cout << "Port opened succesfully" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Port failed to open" << std::endl;
        return 0;
    }
}

/*
*@brief-> Ask for the Amplitude and send to arduino board
*@param-> None
*@return-> None
*/
void SEND_AMP(){
    int amp{0};
    std::cout << "Please input the rocking amplitude(degree):" << std::endl;
    std::cin >> amp;
    std::string amp_str = std::to_string(amp);

    std::string test_str = "Start amp " + amp_str + "\n";
    size_t bytesWritten = my_serial.write(test_str);
    my_serial.flushOutput();
    std::cout << "Bytes sent: " << bytesWritten << std::endl;
}

/*
*@brief-> Ask for the Frequency and send to arduino board
*@param-> None
*@return-> None
*/
void SEND_FRE(){
    double freq{0};
    std::cout << "Please input the frequency(hz): " << std::endl;
    std::cin >> freq;
    std::string freq_str = std::to_string(freq);

    std::string test_str = "Start freq " + freq_str + "\n";
    size_t bytesWritten = my_serial.write(test_str);
    my_serial.flushOutput();
    std::cout << "Bytes sent: " << bytesWritten << std::endl;
}