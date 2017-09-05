#ifndef _RESULT_H_
#define _RESULT_H_

#include <string>

namespace ipclib {

    class Result {
        private:
            int err;
            std::string description;

        public:
            static const int SUCCESS = 0;

            Result(const int theError = SUCCESS, const std::string& theDescription = "");

            int get_error() const { return err;  }
            std::string get_description() const { return description; }
            bool is_succesful() const { return !err; }

            void set_error(const int theError) { err = theError; }
            void set_description(const std::string& theDescription) { description = theDescription; }
            void set(const int theError, const std::string theDescription) { err = theError; description = theDescription; }

            operator bool() const { return is_succesful(); }
    };

}

#endif
