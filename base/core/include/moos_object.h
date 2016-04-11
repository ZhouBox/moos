#ifndef MOOS_OBJECT_H
#define MOOS_OBJECT_H



#include "moos_defines.h"
#include "moos_looper.h"

#include <algorithm>

DEFINE_NAMESPACE_MOOS_BEGIN



class MoosObject
{
public:
    MoosObject(MoosObject* parent = NULL)
        : m_looper(MoosLooper::currentLooper())
        , m_parent(parent)
    {
        if (m_parent) {
            m_parent->addChild(this);
        }
    }


    virtual ~MoosObject()
    {
        if (m_parent) {
            m_parent->removeAchild(this);
            m_parent = NULL;
        }
        clearChildren();

    }

    MoosLooper* eventLooper()
    {
        return m_looper;
    }


protected:


    void addChild(MoosObject* aChild)
    {
        auto ite = std::find(m_children.begin(), m_children.end(), aChild);
        if (ite == m_children.end()) {
            m_children.push_back(aChild);
        }
    }

    void removeAchild(MoosObject* aChild)
    {
        auto ite = std::find(m_children.begin(), m_children.end(), aChild);
        if (ite != m_children.end()) {
            (*ite)->m_parent = NULL;
            m_children.erase(ite);
        }
    }

    void clearChildren()
    {
        std::for_each(m_children.begin(), m_children.end(), [](MoosObject* a_) { a_->m_parent = NULL ; delete a_; });
    }

    MoosLooper* m_looper;
    MoosObject* m_parent;
    std::list<MoosObject*> m_children;
};




DEFINE_NAMESPACE_MOOS_END

#endif // MOOS_OBJECT_H
