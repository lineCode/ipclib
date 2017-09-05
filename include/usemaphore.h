#ifndef _USEMAPHORE_H_
#define _USEMAPHORE_H_

#include <semaphore.h>

#include "posix_obj.h"

namespace ipclib {

    class Semaphore : public Posix_obj {
        private:
            sem_t* sem;

            Result create(std::string& theName, const bool toCreate, const unsigned int theValue);

        public:
            Semaphore(const std::string& theName, const bool toCreate = true, const unsigned int theValue = 1);

            Result wait(int theSeconds = 0, long theNanoSeconds = 0);
            Result signal();
            Result destroy();

            ~Semaphore() { sem_close(sem); }
    };

}

#endif
