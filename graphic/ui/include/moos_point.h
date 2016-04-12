#ifndef MOOS_POINT_H
#define MOOS_POINT_H

#include "moos_defines.h"


DEFINE_NAMESPACE_MOOS_BEGIN


class MoosPoint
{
public:
    MoosPoint(int x_ = 0, int y_ = 0);

    int x() const;
    int y() const;

private:
    int m_x;
    int m_y;
};

DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_POINT_H
