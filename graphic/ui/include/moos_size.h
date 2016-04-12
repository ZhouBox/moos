#ifndef MOOS_SIZE_H
#define MOOS_SIZE_H


#include "moos_defines.h"


DEFINE_NAMESPACE_MOOS_BEGIN


class MoosSize
{
public:
    MoosSize(int w_ = 0, int h_ = 0);

    int height() const;

    int width() const;

private:
    int m_w;
    int m_h;
};


DEFINE_NAMESPACE_MOOS_END



#endif // MOOS_SIZE_H
