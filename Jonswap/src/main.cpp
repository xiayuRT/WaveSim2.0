#include <iostream>
#include <cmath>
#include <vector>
#include "Jonswap.hpp"
#include <matplot/matplot.h>

using namespace matplot;

int main(){
    Jonswap jonswap(120000, 14.4, 10, 0.3);

    const std::vector<float>& time = jonswap.getTIME();
    const std::vector<float>& speed = jonswap.getSPEED();
    const std::vector<float>& waveheight = jonswap.getETA();

    figure(1);
    plot(time, speed);
    hold(on);
    plot(time, waveheight)->color({0.f, 0.7f, 0.9f});
    title("Jonswap Wave in Time Series");
    xlabel("Time(s)");
    ylabel("Wave Height(m)");
    grid(on);
    show();

    // figure(2);
    // const std::vector<float>& freq = jonswap.getFREQ();
    // const std::vector<float>& PSD = jonswap.getPSD();
    // plot(freq, PSD);
    // title("Jonswap");
    // xlabel("Frequency(Hz)");
    // ylabel("PSD(m^2/Hz)");
    // grid(on);
    // show();

    return 0;
}