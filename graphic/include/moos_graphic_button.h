#ifndef MOOS_GRAPHIC_BUTTON_H
#define MOOS_GRAPHIC_BUTTON_H

#include "moos_defines.h"
#include "moos_graphic_item.h"


DEFINE_NAMESPACE_MOOS_BEGIN


class MoosGraphicButton : public MoosGraphicItem
{
public:
    MoosGraphicButton(MoosObject* parent = NULL)
        : MoosGraphicItem(parent)
    {

    }

    void paint();

    void show();

    void repaint();

private:


};





DEFINE_NAMESPACE_MOOS_END



#endif // MOOS_GRAPHIC_BUTTON_H
