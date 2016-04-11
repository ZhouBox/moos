#include "moos_graphic_button.h"
#include "moos_looper.h"

#include <GL/glut.h>


DEFINE_NAMESPACE_MOOS_BEGIN



void MoosGraphicButton::paint()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 25);
    glutInitWindowSize(100, 50);
    glutCreateWindow("Moos Button");
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
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

