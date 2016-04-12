#include "moos_point.h"

DEFINE_NAMESPACE_MOOS_BEGIN

MoosPoint::MoosPoint(int x_, int y_)
    : m_x(x_)
    , m_y(y_)
{

}

int MoosPoint::x() const
{
    return m_x;
}

int MoosPoint::y() const
{
    return m_y;
}


DEFINE_NAMESPACE_MOOS_END
