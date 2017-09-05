#include <fcntl.h>
#include <string.h>

#include "usemaphore.h"

ipclib::Semaphore::Semaphore(const std::string& theName, const bool toCreate, const unsigned int theValue) : Posix_obj(theName) {
    init_err = create(name, toCreate, theValue);
}

ipclib::Result ipclib::Semaphore::create(std::string& theName, const bool toCreate, const unsigned int theValue) {
    int oflag = 0;
    if( toCreate ) oflag = O_CREAT;

    if( (sem = sem_open(theName.c_str(), oflag, DEFAULT_PERMISSION, theValue)) == SEM_FAILED ) return Result(errno, strerror(errno));
    else return Result(Result::SUCCESS);
}

ipclib::Result ipclib::Semaphore::wait(int theSeconds, long theNanoSeconds) {
    if( (theSeconds == 0) && (theNanoSeconds == 0) ) {
        if( sem_wait(sem) != 0 ) return Result(errno, strerror(errno));
    }

    else {
        timespec theTime;
        theTime.tv_sec = theSeconds;
        theTime.tv_nsec = theNanoSeconds;

        if( sem_timedwait(sem, &theTime) != 0 ) return Result(errno, strerror(errno));
    }

    return Result(Result::SUCCESS);
}

ipclib::Result ipclib::Semaphore::signal() {
    if( sem_post(sem) != 0 ) return Result(errno, strerror(errno));
    else return Result(Result::SUCCESS);
}

ipclib::Result ipclib::Semaphore::destroy() {
    if( sem_unlink(name.c_str()) != 0 ) return Result(errno, strerror(errno));
    else return Result(Result::SUCCESS);
}
