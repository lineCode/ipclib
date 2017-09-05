#include "string.h"

#include "msgqueue.h"

ipclib::MsgQueue::MsgQueue(const std::string& theName, const bool toCreate, const bool isNonBlocking, const Protection theProtection) : Posix_obj(theName) {
    init_err = create(name, toCreate, isNonBlocking, theProtection);
}

ipclib::Result ipclib::MsgQueue::create(const std::string& theName, const bool toCreate, const bool isNonBlocking,const Protection theProtection) {
    protection = theProtection;

    int oflag;

    if( protection == READ_ONLY ) oflag = O_RDONLY;
    else if( protection == WRITE_ONLY ) oflag = O_WRONLY;
    else oflag = O_RDWR;

    if( toCreate ) oflag = oflag | O_CREAT;

    if( isNonBlocking ) oflag = oflag | O_NONBLOCK;

    if( (msgq = mq_open(theName.c_str(), oflag, DEFAULT_PERMISSION, NULL)) == (mqd_t)(-1) ) return Result(errno, strerror(errno));
    else return Result(Result::SUCCESS);
}

ipclib::Result ipclib::MsgQueue::get_posixattribute(mq_attr* theAttribute) const {
    if( mq_getattr(msgq, theAttribute) != 0 ) return Result(errno, strerror(errno));
    else return Result(Result::SUCCESS);
}

long ipclib::MsgQueue::get_maxmsg() const {
    mq_attr theAttribute;
    if( !get_posixattribute(&theAttribute) ) return ATTRIBUTE_ERR;
    else return theAttribute.mq_maxmsg;
}

long ipclib::MsgQueue::get_maxmsgsize() const {
    mq_attr theAttribute;
    if( !get_posixattribute(&theAttribute) ) return ATTRIBUTE_ERR;
    else return theAttribute.mq_msgsize;
}

bool ipclib::MsgQueue::is_nonblocking() const {
    mq_attr theAttribute;
    if( !get_posixattribute(&theAttribute) ) return ATTRIBUTE_ERR;
    else if( theAttribute.mq_flags == O_NONBLOCK ) return true;
    else return false;
}

long ipclib::MsgQueue::get_currentmsg() const {
    mq_attr theAttribute;
    if( !get_posixattribute(&theAttribute) ) return ATTRIBUTE_ERR;
    else return theAttribute.mq_curmsgs;
}

ipclib::Result ipclib::MsgQueue::set_nonblocking(const bool isNonBlocking) const {
    mq_attr theAttribute;
    Result res;

    if( (res = get_posixattribute(&theAttribute)) == false ) return res;

    mq_attr newAttribute = theAttribute;

    if( isNonBlocking ) newAttribute.mq_flags = O_NONBLOCK;
    else newAttribute.mq_flags = 0;

    if( mq_setattr(msgq, &newAttribute, &theAttribute) != 0 ) return Result(errno, strerror(errno));
    else return Result(Result::SUCCESS);
}

ipclib::Result ipclib::MsgQueue::receive(std::string& theMsg, int theSeconds, long theNanoSeconds) {
    theMsg = "";
    char* buffer = new char[get_maxmsgsize()];

    if( (theSeconds == 0) && (theNanoSeconds == 0) ) {
        if( mq_receive(msgq, buffer, get_maxmsgsize(), NULL) != 0 ) { delete [] buffer; return Result(errno, strerror(errno)); }
    }

    else {
        timespec theTime;
        theTime.tv_sec = theSeconds;
        theTime.tv_nsec = theNanoSeconds;

        if( mq_timedreceive(msgq, buffer, get_maxmsgsize(), NULL, &theTime) != 0 ) { delete [] buffer; return Result(errno, strerror(errno)); }
    }

    theMsg = buffer;
    delete[] buffer;
    return Result(Result::SUCCESS);
}

ipclib::Result ipclib::MsgQueue::send(const std::string& theMsg, int theSeconds, long theNanoSeconds) {
    if( (theSeconds == 0) && (theNanoSeconds == 0) ) {
        if( mq_send(msgq, theMsg.c_str(), get_maxmsgsize(), 0) != 0 ) return Result(errno, strerror(errno));
    }

    else {
        timespec theTime;
        theTime.tv_sec = theSeconds;
        theTime.tv_nsec = theNanoSeconds;

        if( mq_timedsend(msgq, theMsg.c_str(), get_maxmsgsize(), 0, &theTime) != 0 ) return Result(errno, strerror(errno));
    }

    return Result(Result::SUCCESS);
}

ipclib::Result ipclib::MsgQueue::destroy() {
    if( mq_unlink(name.c_str()) != 0 ) return Result(errno, strerror(errno));
    else return Result(Result::SUCCESS);
}
