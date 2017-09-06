#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_

#include <mqueue.h>

#include "posix_obj.h"

namespace ipclib {

    class MsgQueue : public Posix_obj {
        public:
            enum Protection {
                READ_ONLY,
                WRITE_ONLY,
                READ_AND_WRITE
            };

        private:
            static const long ATTRIBUTE_ERR = -1;
            static const bool NONBLOCKING_ERR = false;

            mqd_t msgq;
            Protection protection;

            Result create(const std::string& theName, const bool toCreate, const bool isNonBlocking, const Protection theProtection);
            Result get_posixattribute(mq_attr* theAttribute) const;

        public:
            MsgQueue(const std::string& theName, const bool toCreate = true, const bool isNonBlocking = false, const Protection theProtection = READ_AND_WRITE);

            bool is_nonblocking() const;
            bool is_empty() const { return !get_currentmsg(); }
            Protection get_protection() const { return protection; }
            long get_maxmsg() const;
            long get_maxmsgsize() const;
            long get_currentmsg() const;

            Result set_nonblocking(const bool isNonBlocking) const;

            Result receive(std::string& theMsg);
            Result receive(std::string& theMsg, int theSeconds, long theNanoSeconds = 0);
            Result send(const std::string& theMsg);
            Result send(const std::string& theMsg, int theSeconds, long theNanoSeconds = 0);
            Result destroy();

            ~MsgQueue() { mq_close(msgq); }
        };

}

#endif
