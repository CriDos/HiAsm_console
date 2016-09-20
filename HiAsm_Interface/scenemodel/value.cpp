//Project
#include "value.h"
#include "cgt/CGTShare.h"

//STL

//Qt

void Value::setType(DataType type)
{
    m_type = type;
}

DataType Value::getType() const
{
    return m_type;
}

uchar Value::toByte() const
{
    if (!canConvert<uchar>())
        return uchar();

    return value<uchar>();
}

int Value::toInt() const
{
    if (!canConvert<int>())
        return int();

    return value<int>();
}

qreal Value::toReal() const
{
    if (!canConvert<qreal>())
        return qreal();

    return value<qreal>();
}

QString Value::toString() const
{
    if (!canConvert<QString>())
        return QString();

    return value<QString>();
}

DataType Value::getTypeArrayItem() const
{
    if (!canConvert<SharedArrayValue>())
        return data_null;

    return value<SharedArrayValue>()->getType();
}

int Value::getArraySize() const
{
    if (!canConvert<SharedArrayValue>())
        return -1;

    return value<SharedArrayValue>()->size();
}

SharedArrayItem Value::getArrayItemByIndex(int index) const
{
    if (!canConvert<SharedArrayValue>())
        return SharedArrayItem();

    const SharedArrayValue &arrayValues = value<SharedArrayValue>();
    if (index < arrayValues->size())
        return arrayValues->at(index);

    return SharedArrayItem();
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
    if (!canConvert<SharedValueFont>())
        return SharedValueFont();

    return value<SharedValueFont>();
}

SharedLinkedElementInfo Value::toLinkedElementInfo() const
{
    if (!canConvert<SharedLinkedElementInfo>())
        return SharedLinkedElementInfo();

    return value<SharedLinkedElementInfo>();
}
