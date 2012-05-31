#include "Event.h"

Event::Event(void)
{
    mut = SDL_CreateMutex();
    cond = SDL_CreateCond();
    triggered = false;
}

void Event::trigger(void)
{
    SDL_LockMutex(mut);
    triggered = true;
    SDL_CondBroadcast(cond);
    SDL_UnlockMutex(mut);
}

void Event::wait(void)
{
    SDL_LockMutex(mut);
    while(!triggered)
        SDL_CondWait(cond, mut);
    SDL_UnlockMutex(mut);
}