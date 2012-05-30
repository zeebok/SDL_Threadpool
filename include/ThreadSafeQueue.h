#ifndef _THREADSAFEQUEUE_H_
#define _THREADSAFEQUEUE_H_

#include <queue>
#include <SDL2/SDL.h>

#include "Updateable.h"

class ThreadSafeQueue
{
    private:
        std::queue<Updateable*> workQueue;
        SDL_mutex* lock;
        SDL_cond* available;

    public:
        ThreadSafeQueue(void);
        ~ThreadSafeQueue(void);

        ThreadSafeQueue(ThreadSafeQueue* q);

        void push(Updateable* job);
        Updateable* pop(void);
};

#endif
