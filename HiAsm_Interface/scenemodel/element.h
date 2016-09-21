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
    ElementFlgs m_flags{};
    bool m_linkIs{};
    Element *m_linkMain{};
    qint32 m_posX{};
    qint32 m_posY{};
    qint32 m_sizeW{};
    qint32 m_sizeH{};
    QString m_codeName;
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

    ElementClass getClassIndex();

    void setFlags(const ElementFlgs &flags);
    ElementFlags getFlags() const;

    void setLinkIs(bool linkIs);
    bool getLinkIs() const;

    void setLinkMain(Element *linkMain);
    Element *getLinkMain() const;

    void setPosX(qint32 posX);
    qint32 getPosX() const;

    void setPosY(qint32 posY);
    qint32 getPosY() const;

    void setSizeW(qint32 sizeW);
    qint32 getSizeW() const;

    void setSizeH(qint32 sizeH);
    qint32 getSizeH() const;

    QString getClassName() const;

    void setCodeName(const QString &name);
    QString getCodeName() const;

    QString getInterfaces() const;
    QString getInherit() const;
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
