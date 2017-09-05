#include "posix_obj.h"

ipclib::Posix_obj::Posix_obj(const std::string theName) {
    name = "/" + theName;
}
