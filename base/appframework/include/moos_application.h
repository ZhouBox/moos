#ifndef MOOSAPPLICATION_H
#define MOOSAPPLICATION_H

#include "moos_defines.h"
#include "moos_object.h"


DEFINE_NAMESPACE_MOOS_BEGIN


class MoosApplication : public MoosObject
{
public:
    MoosApplication(int argc, char *argv[]);

    ~MoosApplication();



    int exec();

private:
    void init(int argc, char* argv[]);

};







DEFINE_NAMESPACE_MOOS_END






#endif // MOOSAPPLICATION_H
