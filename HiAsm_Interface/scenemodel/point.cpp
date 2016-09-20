//Project
#include "point.h"
#include "container.h"
#include "element.h"
#include "scenemodel.h"
#include "cgt/cgt.h"

//STL

//Qt

Point::Point(QObject *parent)
    : QObject(parent)
{
}

Element *Point::getParent() const
{
    return qobject_cast<Element *>(parent());
}

void Point::setType(PointType type)
{
    m_type = type;
}

PointType Point::getType() const
{
    return m_type;
}

void Point::setDataType(DataType dataType)
{
    m_dataType = dataType;
}

DataType Point::getDataType() const
{
    return m_dataType;
}

int Point::getIndex() const
{
    return getParent()->getPointIndexOfType(this);
}

void Point::setName(const QString &name)
{
    m_name = name;
}

QString Point::getName() const
{
    return m_name;
}

void Point::setDpeName(const QString &dpeName)
{
    m_dpeName = dpeName;
}

QString Point::getDpeName() const
{
    return m_dpeName;
}

void Point::setInfo(const QString &info)
{
    m_info = info;
}

QString Point::getInfo() const
{
    return m_info;
}

Point *Point::getLinkPoint() const
{
    return nullptr; //TODO доработать
}

Point *Point::getRLinkPoint() const
{
    if (m_connectPoint.elementId) {
        Element *e = SceneModel::getElementFromEId(m_connectPoint.elementId);
        if (!e)
            return nullptr;

        return e->getPointByName(m_connectPoint.namePoint);
    }

    return nullptr;
}
