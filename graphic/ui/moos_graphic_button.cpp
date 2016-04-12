#include "moos_graphic_button.h"
#include "moos_looper.h"

#include <GL/glut.h>


DEFINE_NAMESPACE_MOOS_BEGIN


MoosGraphicButton::MoosGraphicButton(MoosGraphicItem* parent)
    : MoosGraphicItem(parent)
{
    m_size = MoosSize(120, 45);
}


void MoosGraphicButton::paint()
{
    if (!m_parentItem) {
        glutInitWindowPosition(m_point.x(), m_point.y());
        glutInitWindowSize(m_size.width(), m_size.height());
        glutCreateWindow("Moos Button");
    }
    else {
        m_WindId = glutCreateSubWindow(m_parentItem->windowId(), m_point.x(), m_point.y(),
                                       m_size.width(), m_size.height());
    }
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(0.0f, 0.0f, m_size.height(), m_size.width());
    glFlush();
}


void MoosGraphicButton::show()
{
    m_looper->enqueue(new MoosCommonTask(std::bind(&MoosGraphicButton::paint, this)));
}

void MoosGraphicButton::repaint()
{
    m_looper->enqueue(new MoosCommonTask(std::bind(&MoosGraphicButton::paint, this)));
}





DEFINE_NAMESPACE_MOOS_END

