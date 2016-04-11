#ifndef MOOS_GRAPHIC_ITEM_H
#define MOOS_GRAPHIC_ITEM_H



#include <cassert>
#include <thread>

#include "moos_defines.h"
#include "moos_object.h"


DEFINE_NAMESPACE_MOOS_BEGIN


class MoosGraphicItem : public MoosObject
{
public:

    static std::thread::id s_main_thread_id;


    MoosGraphicItem(MoosObject* parent)
        : MoosObject(parent)
    {
       assert(s_main_thread_id == std::this_thread::get_id());
    }

    virtual void paint() = 0;

private:



};







DEFINE_NAMESPACE_MOOS_END









#endif // MOOS_GRAPHIC_ITEM_H
