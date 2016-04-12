#include "moos_size.h"

DEFINE_NAMESPACE_MOOS_BEGIN


MoosSize::MoosSize(int w_, int h_)
    : m_w(w_)
    , m_h(h_)
{

}

int MoosSize::height() const
{
    return m_h;
}

int MoosSize::width() const
{
    return m_w;
}

DEFINE_NAMESPACE_MOOS_END
