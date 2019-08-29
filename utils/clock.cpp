#include "clock.h"

float Clock::start;

Clock::Clock()
{
    start = -1;
}

void Clock::setStartTime()
{
    Clock::start = ((float)clock())/CLOCKS_PER_SEC;
    cout<<"Iniciando ciclo de relógio. Start: "<<start<<endl;
}

float Clock::calculateDeltaT()
{
    return (((float)clock())/CLOCKS_PER_SEC) - start;
}
