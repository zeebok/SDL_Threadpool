#include "SDLThreadpool.h"

SDLThreadpool::SDLThreadpool()
{

}

SDLThreadpool::~SDLThreadpool()
{

}

void SDLThreadpool::addJob(Updateable* job)
{
    queue.push(job);
}

void SDLThreadpool::closePool(void)
{
    // Do something!
}
