#include <queue>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "Updateable.h"
#include "SDLThreadpool.h"

class Entity : public Updateable
{
    private:
        int health;
    public:
        Entity(int health)
        {
            this->health = health;
        }

        Updateable::ThreadStatus update(void)
        {
            health--;
            fprintf(stderr, "Health: %d\tThread: %d\n", health, (int)SDL_ThreadID());

            return Work;
        }
};

int main(int argc, char** argv)
{
    SDLThreadpool* pool = new SDLThreadpool();
    Entity** test = new Entity*[300];

    for(int i = 0; i < 300; ++i)
    {
        test[i] = new Entity(i+1);
    }

    for(int i = 0; i < 300; ++i)
        pool->addJob(test[i]);

    pool->closePool();

    delete[] test;
    delete pool;

    return 0;
}
