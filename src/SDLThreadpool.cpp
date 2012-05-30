#include "SDLThreadpool.h"

SDLThreadpool::SDLThreadpool()
{
    available = SDL_CreateCond();
}

SDLThreadpool::~SDLThreadpool()
{
    SDL_DestroyCond(available);
}

void SDLThreadpool::addJob(Updateable* job)
{
    queue.push(job);
}

void SDLThreadpool::closePool(void)
{
    // Do something!
}
