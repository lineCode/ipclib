#ifndef _POSIX_OBJ_H_
#define _POSIX_OBJ_H_

#include <string>
#include <sys/stat.h>

#include "result.h"

namespace ipclib {

    class Posix_obj {
        protected:
            static const mode_t DEFAULT_PERMISSION = 0644;
            Result init_err;
            std::string name;

        public:
            Posix_obj(const std::string theName);

            Result get_initresult() const { return init_err; }
            std::string get_posixname() const { return name; }
            std::string get_name() const { return name.substr(1, std::string::npos); }
    };
}

#endif
