#include "result.h"

ipclib::Result::Result(const int theError, const std::string& theDescription) {
    err = theError;
    description = theDescription;
}
