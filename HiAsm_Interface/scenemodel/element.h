#pragma once

//Project
#include "cgt/CGTShare.h"
#include "package/confelement.h"

//STL

//Qt
#include <QObject>
#include <QString>

class SceneModel;
class Container;
class Point;
class Property;

class Element : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Element)

private:
    //Self
    qint32 m_id{};
    qint32 m_userData{};
    ElementClass m_classIndex{};
    ElementFlgs m_flags{};
    qint32 m_group{};
    bool m_linkIs{};
    qint32 m_linkMain{};
    qint32 m_posX{};
    qint32 m_posY{};
    qint32 m_sizeW{};
    qint32 m_sizeH{};
    QString m_className;
    QString m_codeName;
    QString m_interface;
    QString m_inherit;
    QString m_infSub;
    SharedConfElement m_conf;

    //Container
    QVector<Container *> m_containers;

    //Point
    QVector<Point *> m_points;

    //Property
    QVector<Property *> m_properties;

public:
    explicit Element(const QString &name, int id_element, int X, int Y, QObject *parent);

public:
    //Self
    qint32 getId() const;
    Container *getParent() const;

    void setUserData(qint32 userData);
    qint32 getUserData() const;

    void setClassIndex(ElementClass classIndex);
    ElementClass getClassIndex();

    void setFlags(const ElementFlgs &flags);
    ElementFlags getFlags() const;

    void setGroup(qint32 group);
    qint32 getGroup() const;

    void setLinkIs(bool linkIs);
    bool getLinkIs() const;

    void setLinkMain(qint32 linkMain);
    qint32 getLinkMain() const;

    void setPosX(qint32 posX);
    qint32 getPosX() const;

    void setPosY(qint32 posY);
    qint32 getPosY() const;

    void setSizeW(qint32 sizeW);
    qint32 getSizeW() const;

    void setSizeH(qint32 sizeH);
    qint32 getSizeH() const;

    void setClassName(const QString &className);
    QString getClassName() const;

    void setCodeName(const QString &name);
    QString getCodeName() const;

    void setInterface(const QString &interface);
    QString getInterface() const;

    void setInherit(const QString &inherit);
    QString getInherit() const;

    void setInfSub(const QString &infSub);
    QString getInfSub() const;

    //Container
    int getCountContainers() const;
    Container *getContainer() const;
    Container *getContainerByIndex(int index) const;
    Container *addContainer(Container *container);
    void removeContainer(int index);

    //Point
    int getCountPoints() const;
    int getPointIndexOfType(const Point *point) const;
    Point *getPointByIndex(int index) const;
    Point *getPointByName(const QString &name) const;
    Point *addPoint(Point *point);
    void removePoint(int index);

    //Property
    qint32 getCountProps() const;
    Property *getPropertyByIndex(int index) const;
    Property *getPropertyByName(const QString &name) const;
    Property *addProperty(Property *prop);
    void removeProperty(int index);
};
