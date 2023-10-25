#include <iostream>
#include <cmath>
#include <vector>
#include "Jonswap.hpp"
#include <matplot/matplot.h>

using namespace matplot;

int main(){
    Jonswap jonswap(150000, 30, 300, 0.5);
    if(!jonswap.calLIMITED_ETA(300, 0.5, 2.0)){std::cout<<"Failed!"<<std::endl;}

    const std::vector<float>& time = jonswap.getTIME();
    const std::vector<float>& speed = jonswap.getSPEED();
    const std::vector<float>& waveheight = jonswap.getETA();
    std::cout << waveheight[0] << " " << speed[0] << std::endl;

    figure(1);
    plot(time, speed);
    hold(on);
    plot(time, waveheight)->color({0.f, 0.7f, 0.9f});
    title("Jonswap Wave in Time Series");
    xlabel("Time(s)");
    ylabel("Wave Height(m)");
    grid(on);
    show();

    return 0;
}
