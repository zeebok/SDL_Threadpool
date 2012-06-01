#ifndef _SDLTHREADPOOL_H_
#define _SDLTHREADPOOL_H_

#include <stdio.h>
#include <SDL2/SDL.h>

#include "ThreadSafeQueue.h"
#include "Updateable.h"
#include "Event.h"

class SDLThreadpool
{
    private:
        class Poison : public Updateable
        {
            private:
                Event* dead;

            public:
                Poison(Event* dead)
                {
                    this->dead = dead;
                }

                Updateable::ThreadStatus update(void)
                {
                    dead->trigger();

                    return Stop;
                }
        };

        int maxThreads;
        SDL_Thread** pool;
        Updateable::ThreadStatus status;

        ThreadSafeQueue queue;

        static int poolFunc(void* thisPointer);
        void process(void);

    public:
        SDLThreadpool(void);
        ~SDLThreadpool(void);

        void addJob(Updateable* job);
        void closePool(void);
};

#endif
