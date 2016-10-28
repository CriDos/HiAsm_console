//Project
#include "container.h"
#include "element.h"
#include "scenemodel.h"
#include "cgt/cgt.h"

//STL

//Qt

Container::Container(QObject *parent)
    : QObject(parent)
{
}

Element *Container::getParent() const
{
    return qobject_cast<Element *>(parent());
}

QString Container::getName() const
{
    return m_name;
}

void Container::setName(const QString &name)
{
    m_name = name;
}

int Container::getCountElements() const
{
    return m_elements.size();
}

Element *Container::getElementByIndex(int index) const
{
    if (index < m_elements.size())
        return m_elements[index];

    return nullptr;
}

Element *Container::getElementByName(const QString &name) const
{
    for (Element *e : m_elements) {
        if (QString::compare(e->getClassName(), name, Qt::CaseInsensitive) == 0) {
            return e;
        }
    }

    return 0;
}

Element *Container::addElement(Element *element)
{
    m_elements.append(element);
    return element;
}

void Container::removeElement(int index)
{
    m_elements.remove(index);
}
