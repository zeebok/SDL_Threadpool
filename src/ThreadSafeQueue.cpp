#include "ThreadSafeQueue.h"

ThreadSafeQueue::ThreadSafeQueue(void)
{
    lock = SDL_CreateMutex();
}

ThreadSafeQueue::~ThreadSafeQueue(void)
{
    SDL_LockMutex(lock);
    while(!workQueue.empty())
        workQueue.pop();
    SDL_UnlockMutex(lock);

    SDL_DestroyMutex(lock);
}

//ThreadSafeQueue::ThreadSafeQueue(ThreadSafeQueue* q);

void ThreadSafeQueue::push(Updateable* job)
{
    SDL_LockMutex(lock);
    workQueue.push(job);
    SDL_CondSignal(available);
    SDL_UnlockMutex(lock);
}

Updateable* ThreadSafeQueue::pop(void)
{
    SDL_LockMutex(lock);
    while(workQueue.empty())
        SDL_CondWait(available,lock);
    Updateable* top = workQueue.front();
    workQueue.pop();
    SDL_UnlockMutex(lock);

    return top;
}