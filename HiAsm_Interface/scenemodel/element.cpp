//Project
#include "element.h"
#include "container.h"
#include "point.h"
#include "property.h"
#include "scenemodel.h"
#include "cgt/cgt.h"

//STL

//Qt

Element::Element(const QString &name, int id_element, int X, int Y, QObject *parent)
    : QObject(parent)
    , m_id(id_element)
    , m_posX(X)
    , m_posY(Y)
{
    SceneModel::addElementToList(this);

    Package *package = SceneModel::getPackage();
    m_conf = package->getElementByName(name);
    copyFromConf();
}

void Element::copyFromConf()
{
    const PointConfList &list = m_conf->getPoints();
    for (const SharedPointConf &p : list) {
        addPoint(p);
    }
}

qint32 Element::getId() const
{
    return m_id;
}

Container *Element::getParent() const
{
    return qobject_cast<Container *>(parent());
}

void Element::setUserData(qint32 userData)
{
    m_userData = userData;
}

qint32 Element::getUserData() const
{
    return m_userData;
}

ElementClass Element::getClassIndex()
{
    return m_conf->getClass();
}

void Element::setFlags(const ElementFlgs &flags)
{
    m_flags = flags;
}

ElementFlags Element::getFlags() const
{
    return ElementFlags(qint32(m_flags));
}

void Element::setLinkIs(bool linkIs)
{
    m_linkIs = linkIs;
}

bool Element::getLinkIs() const
{
    return m_linkIs;
}

void Element::setLinkMain(Element *linkMain)
{
    m_linkMain = linkMain;
}

Element *Element::getLinkMain() const
{
    return m_linkMain;
}

void Element::setPosX(qint32 posX)
{
    m_posX = posX;
}

qint32 Element::getPosX() const
{
    return m_posX;
}

void Element::setPosY(qint32 posY)
{
    m_posY = posY;
}

qint32 Element::getPosY() const
{
    return m_posY;
}

void Element::setSizeW(qint32 sizeW)
{
    m_sizeW = sizeW;
}

qint32 Element::getSizeW() const
{
    return m_sizeW;
}

void Element::setSizeH(qint32 sizeH)
{
    m_sizeH = sizeH;
}

qint32 Element::getSizeH() const
{
    return m_sizeH;
}

QString Element::getClassName() const
{
    return m_conf->getName();
}

void Element::setCodeName(const QString &name)
{
    m_codeName = name;
}

QString Element::getCodeName() const
{
    return m_codeName;
}

QString Element::getInterfaces() const
{
    return m_conf->getInterfaces();
}

QString Element::getInherit() const
{
    return m_conf->getInherits().join(',');
}

QString Element::getInfSub() const
{
    return m_conf->getSub();
}

int Element::getCountContainers() const
{
    return m_containers.size();
}

Container *Element::getContainer() const
{
    if (m_containers.isEmpty())
        return nullptr;

    return m_containers[0];
}

Container *Element::getContainerByIndex(int index) const
{
    if (index < m_containers.size())
        return m_containers[index];
    else
        return nullptr;
}

Container *Element::addContainer(Container *container)
{
    m_containers.append(container);
    return container;
}

void Element::removeContainer(int index)
{
    m_containers.remove(index);
}

int Element::getCountPoints() const
{
    return m_points.size();
}

int Element::getPointIndexOfType(const Point *point) const
{
    if (!point)
        return -1;

    int idx = 0;
    for (const Point *p : m_points) {
        if (p == point) {
            return idx;
        } else if (point->getType() == p->getType()) {
            ++idx;
        }
    }

    return -1;
}

Point *Element::getPointByIndex(int index) const
{
    if (index < m_points.size())
        return m_points[index];
    else
        return nullptr;
}

Point *Element::getPointByName(const QString &name) const
{
    for (Point *p : m_points) {
        if (QString::compare(p->getName(), name, Qt::CaseInsensitive) == 0) {
            return p;
        }
    }

    return nullptr;
}

Point *Element::addPoint(const SharedPointConf conf)
{
    m_points.append();
    return point;
}

void Element::removePoint(int index)
{
    m_points.remove(index);
}

qint32 Element::getCountProps() const
{
    return m_properties.size();
}

Property *Element::getPropertyByIndex(int index) const
{
    if (index < m_properties.size())
        return m_properties[index];

    return nullptr;
}

Property *Element::getPropertyByName(const QString &name) const
{
    for (Property *p : m_properties) {
        if (QString::compare(p->getName(), name, Qt::CaseInsensitive) == 0) {
            return p;
        }
    }

    return nullptr;
}

Property *Element::addProperty(Property *prop)
{
    m_properties.append(prop);
    return prop;
}

void Element::removeProperty(int index)
{
    m_points.remove(index);
}
