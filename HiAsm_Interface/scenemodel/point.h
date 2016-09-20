#pragma once

//Project
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QDebug>

class SceneModel;
class Element;

class Point : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Point)

private:
    //Self
    PointType m_type{};
    DataType m_dataType{};
    QString m_name;
    QString m_dpeName;
    QString m_info;
    struct {
        qint32 elementId{};
        QString namePoint;
    } m_connectPoint;

public:
    explicit Point(QObject *parent);

public:
    //Self
    Element *getParent() const;

    void setType(PointType type);
    PointType getType() const;

    void setDataType(DataType dataType);
    DataType getDataType() const;

    int getIndex() const;

    void setName(const QString &name);
    QString getName() const;

    void setDpeName(const QString &dpeName);
    QString getDpeName() const;

    void setInfo(const QString &info);
    QString getInfo() const;

    Point *getLinkPoint() const;
    Point *getRLinkPoint() const;
};
