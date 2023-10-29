#include <iostream>
#include <limits>

#include "Jonswap.hpp"

#define PULLEY_DIAMETER_CM 4.85
#define CNT_PER_ROUND 800
#define M_TO_CM 100
#define M_PER_SEC_TO_RPM(x) ((x) * 60 * M_TO_CM / (M_PI * PULLEY_DIAMETER_CM)) 
#define M_TO_CNT(x) ((x) * M_TO_CM * CNT_PER_ROUND / (M_PI * PULLEY_DIAMETER_CM))
#define CNT_TO_CM(x) ((x) * M_PI * PULLEY_DIAMETER_CM / CNT_PER_ROUND)

int main(){
        while(true);
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
				case(1): std::cout << "Mode 1."<<std::endl; break;
				case(2): Jonswap jonswap(30000, 20, 30, 0.5, 1.9);
                         const std::vector<float>& time = jonswap.getTIME();
                         const std::vector<float>& speed = jonswap.getSPEED();
                         const std::vector<float>& waveheight = jonswap.getETA();
                         std::cout << "Init wave height : " << waveheight[0]*100*800/M_PI/4.85 << std::endl;
                         std::cout << "Init wave height : " <<  M_TO_CNT(waveheight[0])<< std::endl;
                         for(size_t i=0;i<waveheight.size();i++){std::cout <<speed[i] * 100 * 60 / M_PI / 4.85 << " "<< M_PER_SEC_TO_RPM(speed[i])<<std::endl;}
                         break;
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
}