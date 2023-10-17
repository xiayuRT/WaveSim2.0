#include <iostream>
#include <cmath>
#include <vector>
#include "Jonswap.hpp"
#include <matplot/matplot.h>

using namespace matplot;

int main(){
    std::cout << "start" << std::endl;
    Jonswap jonswap(5, 8);
    const std::vector<float>& psdValues = jonswap.getFreq();
    std::cout << jonswap.getPSD().size() << std::endl;
    for (size_t i = 0; i < psdValues.size(); i++) {
        std::cout << "PSD[" << i << "] = " << psdValues[i] << std::endl;
    }

    std::vector<double> x = linspace(0, 2 * pi);
    std::vector<double> y = transform(x, [](auto x) { return sin(x); });

    plot(x, y, "-o");
    hold(on);
    plot(x, transform(y, [](auto y) { return -y; }), "--xr");
    plot(x, transform(x, [](auto x) { return x / pi - 1.; }), "-:gs");
    plot({1.0, 0.7, 0.4, 0.0, -0.4, -0.7, -1}, "k");

    show();

    return 0;
}