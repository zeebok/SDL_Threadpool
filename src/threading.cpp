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

        int update(void)
        {
            health--;
            printf("Health: %d\n", health);

            return 0;
        }
};

int main(int argc, char** argv)
{
    SDLThreadpool* pool = new SDLThreadpool();
    Entity** test = new Entity*[30];

    for(int i = 0; i < 30; ++i)
    {
        test[i] = new Entity(i+1);
    }

    for(int i = 0; i < 30; ++i)
        pool->addJob(test[i]);

    pool->closePool();

    delete[] test;
    delete pool;

    return 0;
}
