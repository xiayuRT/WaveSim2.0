#include <iostream>
#include <vector>
#include "Jonswap.hpp"
#include <fstream>


int main(){
    Jonswap jonswap(300000, 13.6, 1200, 0.5);
    jonswap.calLIMITED_ETA(1200, 0.5, 2.0);

    const std::vector<float>& time = jonswap.getTIME();
    const std::vector<float>& speed = jonswap.getSPEED();
    const std::vector<float>& waveheight = jonswap.getETA();

    std::ofstream csvFile("output.csv");
    if(!csvFile.is_open()){
        std::cerr<<"Unable to open csv file"<<std::endl;
        return 1;
    }

    for (size_t i=0; i<time.size(); i++){
        csvFile << time[i];
        csvFile << ",";
        csvFile << speed[i];
        csvFile << ",";
        csvFile << waveheight[i];
        csvFile << "\n";
    }

    csvFile.close();

    return 0;
}
