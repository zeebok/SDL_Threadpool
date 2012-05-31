#ifndef _EVENT_H_
#define _EVENT_H_

#include <SDL2/SDL.h>

class Event
{
    private:
        SDL_mutex* mut;
        SDL_cond* cond;
        bool triggered;

    public:
        Event(void);

        void trigger(void);
        void wait(void);
};

#endif