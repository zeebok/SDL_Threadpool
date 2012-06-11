#include "ThreadSafeQueue.h"

template <class T>
ThreadSafeQueue<T>::ThreadSafeQueue(void)
{
    lock = SDL_CreateMutex();
    available = SDL_CreateCond();
}

template <class T>
ThreadSafeQueue<T>::~ThreadSafeQueue(void)
{
    SDL_LockMutex(lock);
    while(!workQueue.empty())
        workQueue.pop();
    SDL_UnlockMutex(lock);

    SDL_DestroyMutex(lock);
    SDL_DestroyCond(available);
}

//ThreadSafeQueue::ThreadSafeQueue(ThreadSafeQueue* q);

template <class T>
void ThreadSafeQueue<T>::push(T job)
{
    SDL_LockMutex(lock);
    workQueue.push(job);
    SDL_CondBroadcast(available);
    SDL_UnlockMutex(lock);
}

template <class T>
T ThreadSafeQueue<T>::pop(void)
{
    SDL_LockMutex(lock);
    while(workQueue.empty())
        SDL_CondWait(available,lock);
    T top = workQueue.front();
    workQueue.pop();
    SDL_UnlockMutex(lock);

    return top;
}
