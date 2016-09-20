#pragma once
//Project
#include "valuetypes.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QVariant>

class Value
{
    //Self
    DataType m_type = data_null;
    QVariant m_value;

public:
    explicit Value(DataType type = data_null, const QVariant &value = QVariant());

public:
    //Self
    void setType(DataType type);
    DataType getType() const;

    //Value
    void setValue(const QVariant &value);
    QVariant getValue() const;

    //Byte
    uchar toByte() const;

    //Int
    qint32 toInt() const;

    //Real
    qreal toReal() const;

    //String
    QString toString() const;

    //Array
    DataType getTypeArrayItem() const;
    int getArraySize() const;
    SharedArrayItem getArrayItemByIndex(int index) const;
    QString getArrayItemName(int index) const;

    //Font
    SharedValueFont toFont() const;

    //LinkedElementInfo
    SharedLinkedElementInfo toLinkedElementInfo() const;
};
