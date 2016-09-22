#pragma once

//Project
#include "value.h"
#include "valuetypes.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>

class SceneModel;
class Element;

class Property : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Property)

private:
    //Self
    QString m_name;
    bool m_isDefProp{};

    //Value
    Value m_value;

public:
    explicit Property(DataType type = data_null,
                      const QVariant &data = QVariant(),
                      const QString &name = QString(),
                      QObject *parent = 0);

public:
    //Self
    void setName(const QString &name);
    QString getName() const;
    Element *getParent() const;
    bool getIsDefProp() const;

    void setType(DataType type);
    DataType getType() const;

    //Value
    void setValue(const QVariant &data);
    Value *value();
};
