#ifndef CLOCK_H
#define CLOCK_H
#include <iostream>
#include <map>
#include <string>
#include <ctime>
#include <cmath>
using namespace std;

class Clock
{
private:
    static float start;

public:
    Clock();

    static void setStartTime();

    static float calculateDeltaT();
};

#endif //CLOCK_H
