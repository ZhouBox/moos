#include "moos_application.h"

#include <GL/glut.h>


DEFINE_NAMESPACE_MOOS_BEGIN



MoosApplication::MoosApplication(int argc, char *argv[])
{
    int _t = argc;
    glutInit(&_t, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
}


MoosApplication::~MoosApplication()
{

}




int MoosApplication::exec()
{
    m_looper->exec();
    return 0;
}


DEFINE_NAMESPACE_MOOS_END
