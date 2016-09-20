#pragma once

//Project
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>

class SceneModel;
class Element;

class Container : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Container)

private:
    //Self
    QString m_name;

    //Element
    QVector<Element *> m_elements;

public:
    explicit Container(QObject *parent);

public:
    //Self
    Element *getParent() const;
    void setName(const QString &name);
    QString getName() const;

    //Element
    int getCountElements() const;
    Element *getElementByIndex(int index) const;
    Element *getElementByName(const QString &name) const;
    Element *addElement(Element *element);
    void removeElement(int index);
};
