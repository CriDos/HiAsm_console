#pragma once
//Project
#include "types.h"

//STL

//Native

//Qt
#include <QtCore>

class ConfElement;
typedef QSharedPointer<ConfElement> SharedConfElement;
typedef QList<SharedConfElement> ConfElementList;

class ConfElement
{
private:
    enum TypeSection {
        ts_undefine,
        ts_abouts,
        ts_types,
        ts_edit,
        ts_properties,
        ts_points,
    };

private:
    //Self
    QString m_pathConf;
    QString m_name;
    //QIcon m_icon;

    //Inherited
    bool m_isInherited = false;

    //About
    QString m_version;
    QString m_author;
    QString m_mail;

    //Type
    ElementClass m_class;
    QStringList m_inherit;
    ConfElementList m_inheritList;
    QString m_sub;
    QString m_info;
    QString m_propIcon;
    QString m_view;
    QString m_tab;
    QString m_interfaces;

    //Edit
    QString m_editClass;

    //Group
    MapPropGroup m_group;

    //Properties
    PropConfList m_propList;

    //Points
    PointConfList m_pointList;
    PointConfList m_hiddenPointList;

public:
    explicit ConfElement(const QString &pathConf);

private:
    void initConfElement();
    void loadConf();
    void parseAbout(const QStringList &list);
    void parseTypes(const QStringList &list);
    void parsePropValue(const QString &sline, SharedPropConf conf);
    void parseProperties(const QStringList &list);
    void parsePoints(const QStringList &list);

public:
    QString getName() const;
    void setName(const QString &name);

    QString getVersion() const;
    void setVersion(const QString &version);

    QString getAuthor() const;
    void setAuthor(const QString &author);

    QString getMail() const;
    void setMail(const QString &mail);

    ElementClass getClass() const;
    void setClass(const ElementClass eclass);

    QStringList getInherits() const;
    void setInherits(const QStringList &inherit);

    QString getSub() const;
    void setSub(const QString &sub);

    QString getInfo() const;
    void setInfo(const QString &info);

    QString getPropIcon() const;
    void setPropIcon(const QString &propIcon);

    QString getView() const;
    void setView(const QString &view);

    QString getTab() const;
    void setTab(const QString &tab);

    QString getInterfaces() const;
    void setInterfaces(const QString &interfaces);

    QString getEditClass() const;
    void setEditClass(const QString &editClass);

    //Inherits
    void addInheritElements(Package *pack);

    PropConfList getProperties() const;
    PointConfList getPoints() const;
    PointConfList getHiddenPoints() const;
};
