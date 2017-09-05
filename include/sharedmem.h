#ifndef _SHAREDMEM_H_
#define SHAREDMEM_H_

#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "posix_obj.h"

namespace ipclib {

    template<class T> class SharedMem : public Posix_obj {
        public:
            enum Protection {
                READ_ONLY,
                WRITE_ONLY,
                READ_AND_WRITE
            };


        private:
            T* obj_addr;
            Protection protection;

            Result create(const std::string& theName, const bool toCreate, const Protection theProtection);

        public:
            SharedMem(const std::string& theName, const bool toCreate = true, const Protection theProtection = READ_AND_WRITE);

            Protection get_protection() const { return protection; }
            T& get_value() const { return *obj_addr; }

            void set_value(const T& theValue) { *obj_addr = theValue; }

            Result destroy();

            ~SharedMem() { munmap(obj_addr, sizeof(T)); }
    };

    template<class T> SharedMem<T>::SharedMem(const std::string& theName, const bool toCreate, const Protection theProtection) : Posix_obj(theName) {
        init_err = create(name, toCreate, theProtection);
    }

    template<class T> Result SharedMem<T>::create(const std::string& theName, const bool toCreate, const Protection theProtection) {
        protection = theProtection;

        int oflag;
        if( theProtection == READ_ONLY ) oflag = O_RDONLY;
        else oflag = O_RDWR;

        if( toCreate ) oflag = oflag | O_CREAT;

        int fd;
        if( (fd = shm_open(theName.c_str(), oflag, DEFAULT_PERMISSION)) != 0 ) return Result(errno, strerror(errno));

        if( (ftruncate(fd, sizeof(T))) != 0 ) return Result(errno, strerror(errno));

        int prot_oflag;
        if( theProtection == READ_ONLY ) prot_oflag = PROT_READ;
        else if( theProtection == WRITE_ONLY ) prot_oflag = PROT_WRITE;
        else prot_oflag = PROT_READ | PROT_WRITE;

        if( (obj_addr = (T*)(mmap(NULL, sizeof(T), prot_oflag, MAP_SHARED, fd, NULL))) == (void*)(-1) ) return Result(errno, strerror(errno));

        if( close(fd) != 0 ) return Result(errno, strerror(errno));

        return Result(Result::SUCCESS);
    }

    template<class T> Result SharedMem<T>::destroy() {
        if( shm_unlink(name.c_str()) != 0 ) return Result(errno, strerror(errno));
        else return Result(Result::SUCCESS);
    }
}

#endif
