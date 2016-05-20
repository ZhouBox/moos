#ifndef MOOS_INPUTMANAGER_H
#define MOOS_INPUTMANAGER_H

#include "moos_defines.h"
#include "moos_thread.h"
#include "moos_signal.h"


DEFINE_NAMESPACE_MOOS_BEGIN

class MoosEventHub;
class MoosRawEvent;

class MoosInputReader : public MoosThread
{
public:
    MoosInputReader(MoosEventHub* hub_);
    bool threadRun();
    MOOS_SIGNAL(MoosRawEvent*, size_t) SIG_RAWEVENTS;
private:
    MoosEventHub *m_eventHub;

};







class MoosInputManager
{
public:
    MoosInputManager();
    void start();
public:
    MoosEventHub *m_eventHub;
    MoosInputReader *m_inputreader;
};


DEFINE_NAMESPACE_MOOS_END


#endif // MOOS_INPUTMANAGER_H
