#include "SDLThreadpool.h"

SDLThreadpool::SDLThreadpool()
{
    maxThreads = SDL_GetCPUCount();
    pool = new SDL_Thread*[maxThreads];

    for(int i = 0; i < maxThreads; ++i)
    {
        char name[32];
        sprintf(name,"pool%d", i);
        SDL_CreateThread(poolFunc, name, this);
    }
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
    Event* death[maxThreads];

    for(int i = 0; i < maxThreads; ++i)
    {
        death[i] = new Event();
        queue.push(new Poison(death[i]));
    }

    for(int i = 0; i < maxThreads; ++i)
    {
        death[i]->wait();
    }
}

int SDLThreadpool::poolFunc(void* thisPointer)
{
    SDLThreadpool* tp = (SDLThreadpool*) thisPointer;
    tp->process();

    return 0;
}

void SDLThreadpool::process(void)
{
    while(status != Stop)
    {
        Updateable* job = queue.pop();
        job->update();
    }
}