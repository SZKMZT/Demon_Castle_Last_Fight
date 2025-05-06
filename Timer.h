#include <SDL.h>
using namespace std;

class Timer
{
    public:
        Timer();
        void settime();
        void setstarttime();
        Uint32 getlasttime();
        Uint32 getdeltatime();
        Uint32 gettime();
    private:
        Uint32 starttime;
        Uint32 lasttime;
        Uint32 deltatime;
};

Timer::Timer()
{
    starttime = SDL_GetTicks();
    lasttime = starttime;
    deltatime = 0;
}

void Timer::settime()
{
    lasttime = SDL_GetTicks();
    starttime = SDL_GetTicks();
} 

void Timer::setstarttime()
{
    starttime = SDL_GetTicks();
}

Uint32 Timer::getlasttime()
{
    return lasttime;
}

Uint32 Timer::getdeltatime()
{
    deltatime = SDL_GetTicks() - lasttime;
    lasttime = SDL_GetTicks();
    return deltatime;
}

Uint32 Timer::gettime()
{
    return SDL_GetTicks() - starttime;
}
