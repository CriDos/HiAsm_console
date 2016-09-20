//Project
#include "value.h"
#include "cgt/CGTShare.h"

//STL

//Qt

Value::Value(DataType type, const QVariant &value)
    : m_type(type)
    , m_value(value)
{
}

void Value::setType(DataType type)
{
    m_type = type;
}

DataType Value::getType() const
{
    return m_type;
}

void Value::setValue(const QVariant &value)
{
    m_value = value;
}

QVariant Value::getValue() const
{
    return m_value;
}

uchar Value::toByte() const
{
    if (!m_value.canConvert<uchar>())
        return uchar();

    return m_value.value<uchar>();
}

qint32 Value::toInt() const
{
    if (!m_value.canConvert<qint32>())
        return qint32();

    return m_value.value<qint32>();
}

qreal Value::toReal() const
{
    if (!m_value.canConvert<qreal>())
        return qreal();

    return m_value.value<qreal>();
}

QString Value::toString() const
{
    if (!m_value.canConvert<QString>())
        return QString();

    return m_value.value<QString>();
}

DataType Value::getTypeArrayItem() const
{
    if (!m_value.canConvert<SharedArrayValue>())
        return data_null;

    return m_value.value<SharedArrayValue>()->getType();
}

int Value::getArraySize() const
{
    if (!m_value.canConvert<SharedArrayValue>())
        return -1;

    return m_value.value<SharedArrayValue>()->size();
}

SharedArrayItem Value::getArrayItemByIndex(int index) const
{
    if (!m_value.canConvert<SharedArrayValue>())
        return SharedArrayValue();

    const SharedArrayValue &arrayValues = m_value.value<SharedArrayValue>();
    if (index < arrayValues->size())
        return arrayValues->at(index);

    return SharedArrayValue();
}

QString Value::getArrayItemName(int index) const
{
    const SharedArrayItem arrItem = getArrayItemByIndex(index);
    if (!arrItem)
        return QString();

    return arrItem->name;
}

SharedValueFont Value::toFont() const
{
    if (!m_value.canConvert<SharedValueFont>())
        return SharedValueFont();

    return m_value.value<SharedValueFont>();
}

SharedLinkedElementInfo Value::toLinkedElementInfo() const
{
    if (!m_value.canConvert<SharedLinkedElementInfo>())
        return SharedLinkedElementInfo();

    return m_value.value<SharedLinkedElementInfo>();
}
