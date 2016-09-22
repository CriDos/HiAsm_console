//Project
#include "property.h"
#include "element.h"
#include "valuetypes.h"
#include "scenemodel.h"
#include "cgt/cgt.h"

//STL
#include <limits>

//Qt
#include <QTemporaryFile>
#include <QDebug>
#include <QUuid>

Property::Property(DataType type, const QVariant &data, const QString &name, QObject *parent)
    : QObject(parent)
{
    m_name = name;
    m_value.setType(type);
    m_value.setValue(data);
}

void Property::setName(const QString &name)
{
    m_name = name;
}

QString Property::getName() const
{
    return m_name;
}

Element *Property::getParent() const
{
    return qobject_cast<Element *>(parent());
}

void Property::setType(DataType type)
{
    m_value.setType(type);
}

DataType Property::getType() const
{
    return m_value.getType();
}

bool Property::getIsDefProp() const
{
    return m_isDefProp;
}

void Property::setValue(const QVariant &data)
{
    m_value.setValue(data);
}

Value *Property::value()
{
    return &m_value;
}
