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

Property::Property(DataType type, const QVariant &data, const QString &name)
{
    m_name = name;
    m_type = type;
    m_value.setType(type);
    m_value.setValue(data);
    m_value.setName(name);
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
    m_type = type;
}

DataType Property::getType() const
{
    return m_type;
}

void Property::setIsDefProp(bool value)
{
    m_isDefProp = value;
}

bool Property::getIsDefProp() const
{
    return m_isDefProp;
}

void Property::setValue(DataType type, const QVariant &data, const QString &name, DataType arrayType)
{
    m_value.setType(type);
    m_value.setValue(data);
    m_value.setName(name);
    m_value.setSubType(arrayType);
}

Value *Property::getValue() const
{
    return &m_value;
}

uchar Property::toByte() const
{
    return m_value.toByte();
}

qint32 Property::toInt() const
{
    return m_value.toInt();
}

qreal Property::toReal() const
{
    return m_value.toReal();
}

QString Property::toString() const
{
    return m_value.toString();
}

SharedLinkedElementInfo Property::toLinkedElementInfo() const
{
    return m_value.toLinkedElementInfo();
}
