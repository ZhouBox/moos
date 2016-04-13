#include "moos_graphic_button.h"
#include "moos_looper.h"
#include "moos_log.h"

#include <GL/glut.h>


DEFINE_NAMESPACE_MOOS_BEGIN



MoosGraphicButton::MoosGraphicButton(MoosGraphicItem* parent)
    : MoosGraphicItem(parent)
{
    m_point = MoosPoint(500, 500);
    m_size = MoosSize(120, 45);
}


void MoosGraphicButton::paint()
{
    if (!m_parentItem) {
        glutInitWindowPosition(m_point.x(), m_point.y());
        glutInitWindowSize(m_size.width(), m_size.height());
        m_WindId = glutCreateWindow("Moos Button");
    }
    else {
        m_WindId = glutCreateSubWindow(m_parentItem->windowId(), m_point.x(), m_point.y(),
                                       m_size.width(), m_size.height());
    }
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2i(m_point.x(), m_point.y());
        glVertex2i(m_point.x() + m_size.width(), m_point.y());
        glVertex2i(m_point.x() + m_size.width(), m_point.y() + m_size.height());
        glVertex2i(m_point.x(), m_point.y() + m_size.height());
    glEnd();
//    glRecti(m_point.x(), m_point.y(), m_point.x() + m_size.width(), m_point.y() + m_size.height());
    MOOS_DEBUG_LOG(m_point.x(), m_point.y(), m_point.x() + m_size.width(), m_point.y() + m_size.height());
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

