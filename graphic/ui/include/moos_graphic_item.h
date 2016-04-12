#ifndef MOOS_GRAPHIC_ITEM_H
#define MOOS_GRAPHIC_ITEM_H



#include <cassert>
#include <thread>

#include "moos_defines.h"
#include "moos_object.h"

#include "moos_point.h"
#include "moos_size.h"


DEFINE_NAMESPACE_MOOS_BEGIN


class MoosGraphicItem : public MoosObject
{
public:

    static std::thread::id s_main_thread_id;


    MoosGraphicItem(MoosGraphicItem* parent = NULL)
        : MoosObject(parent)
        , m_WindId(-1)
        , m_parentItem(parent)
    {
       assert(s_main_thread_id == std::this_thread::get_id());
    }

    virtual void paint() = 0;


    int windowId() const
    {
        return m_WindId;
    }





protected:
    int m_WindId;
    MoosGraphicItem* m_parentItem;
    MoosPoint m_point;
    MoosSize m_size;



};







DEFINE_NAMESPACE_MOOS_END









#endif // MOOS_GRAPHIC_ITEM_H
