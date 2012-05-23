#include <queue>
#include <stdio.h>
#include <SDL2/SDL.h>

class Updateable
{
    public:
        virtual int update(void) = 0;
};

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

class Threadpool
{
    private:
        int maxThreads;
        SDL_mutex* queueMutex;
        SDL_cond* empty;

        SDL_Thread** pool;

        std::queue<Updateable*> workQueue;

        class Poison : public Updateable
        {
            private:
                int id;
            public:
                Poison(int id)
                {
                    this->id = id;
                }
                int update(void)
                {
                    // Somehow kill the thread
                    printf("POISON\n");
                    return id;
                }
        };

        void process(void)
        {
            int running = true;
            while(running)
            {
                SDL_LockMutex(queueMutex);
                if(workQueue.empty())
                {
                    printf("Waiting thread %d\tQueue Size: %d\n", (int)SDL_ThreadID(), (int)workQueue.size());
                    SDL_CondWaitTimeout(empty, queueMutex, 1000);
                }
                else
                {
                    Updateable* job = workQueue.front();
                    workQueue.pop();

                    int result = job->update();

                    if((int)SDL_ThreadID() == result)
                    {
                        printf("Poisoned\n");
                        running = false;
                    }
                    else if(0 != result)
                    {
                        printf("Not poisoned\n");
                        addQueue(job);
                        SDL_Delay(1000);
                    }
                }
            }
            SDL_UnlockMutex(queueMutex);
        }

        static int poolFunc(void* thisPointer)
        {
            Threadpool* tp = (Threadpool*) thisPointer;
            tp->process();

            return 0;
        }

    public:
        Threadpool(void)
        {
            maxThreads = SDL_GetCPUCount();
            printf("Number of threads: %d\n", maxThreads);

            queueMutex = SDL_CreateMutex();
            empty = SDL_CreateCond();

            pool = new SDL_Thread*[maxThreads];

            for(int i = 0; i < maxThreads; ++i)
            {
                char name[32];
                sprintf(name, "pool%d", i);
                SDL_CreateThread(poolFunc, name, this);
            }
        }

        ~Threadpool(void)
        {
            SDL_DestroyMutex(queueMutex);
            SDL_DestroyCond(empty);

            delete[] pool;
        }

        void addQueue(Updateable* newJob)
        {
            SDL_LockMutex(queueMutex);
            bool signal = (workQueue.empty())?true:false;
            workQueue.push(newJob);
            printf("Queue Size: %d\n", (int)workQueue.size());
            if(signal)
                SDL_CondSignal(empty);
            SDL_UnlockMutex(queueMutex);
        }

        void closePool(void)
        {
            for(int i = 0; i < maxThreads; ++i)
            {
                addQueue(new Poison(SDL_GetThreadID(pool[i])));
                SDL_WaitThread(pool[i], NULL);
            }
        }
};

int main(int argc, char** argv)
{
    Threadpool* pool = new Threadpool();
    Entity* test = new Entity(50);
    for(int i = 0; i < 30; ++i)
        pool->addQueue(test);

    pool->closePool();

    delete pool;

    return 0;
}
