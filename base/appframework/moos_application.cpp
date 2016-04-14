#include "moos_application.h"

#include <GL/glut.h>


DEFINE_NAMESPACE_MOOS_BEGIN



MoosApplication::MoosApplication(int argc, char *argv[])
{
    init(argc, argv);
}


MoosApplication::~MoosApplication()
{

}




int MoosApplication::exec()
{
    return m_looper->exec();
}

void MoosApplication::init(int argc, char* argv[])
{
    int _t = argc;
    glutInit(&_t, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
}


DEFINE_NAMESPACE_MOOS_END
