#ifndef _SDLTHREADPOOL_H_
#define _SDLTHREADPOOL_H_

#include <SDL2/SDL.h>

#include "ThreadSafeQueue.h"
#include "Updateable.h"

class SDLThreadpool
{
    private:
        int maxThreads;
        SDL_Thread** pool;

        ThreadSafeQueue queue;

    public:
        SDLThreadpool(void);
        ~SDLThreadpool(void);

        void addJob(Updateable* job);
        void closePool(void);
};

#endif
