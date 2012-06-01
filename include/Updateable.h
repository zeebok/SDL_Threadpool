#ifndef _UPDATEABLE_H_
#define _UPDATEABLE_H_

class Updateable
{
    public:
        typedef enum {Work, Stop} ThreadStatus;
        virtual ThreadStatus update(void) = 0;
};

#endif
