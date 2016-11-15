//Project
#include "confelement.h"
#include "package.h"
#include "cgt/CGTShare.h"
#include "scenemodel/valuetypes.h"

//STL

//Native

//Qt

QString ConfElement::getName() const
{
    return m_name;
}

void ConfElement::setName(const QString &name)
{
    m_name = name;
}

QString ConfElement::getVersion() const
{
    return m_version;
}

void ConfElement::setVersion(const QString &version)
{
    m_version = version;
}

QString ConfElement::getAuthor() const
{
    return m_author;
}

void ConfElement::setAuthor(const QString &author)
{
    m_author = author;
}

QString ConfElement::getMail() const
{
    return m_mail;
}

void ConfElement::setMail(const QString &mail)
{
    m_mail = mail;
}

ElementClass ConfElement::getClass() const
{
    return m_class;
}

void ConfElement::setClass(const ElementClass eclass)
{
    m_class = eclass;
}

QStringList ConfElement::getInherits() const
{
    return m_inherit;
}

void ConfElement::setInherits(const QStringList &inherit)
{
    m_inherit = inherit;
}

QString ConfElement::getSub() const
{
    return m_sub;
}

void ConfElement::setSub(const QString &sub)
{
    m_sub = sub;
}

QString ConfElement::getInfo() const
{
    return m_info;
}

void ConfElement::setInfo(const QString &info)
{
    m_info = info;
}

QString ConfElement::getPropIcon() const
{
    return m_propIcon;
}

void ConfElement::setPropIcon(const QString &propIcon)
{
    m_propIcon = propIcon;
}

QString ConfElement::getView() const
{
    return m_view;
}

void ConfElement::setView(const QString &view)
{
    m_view = view;
}

QString ConfElement::getTab() const
{
    return m_tab;
}

void ConfElement::setTab(const QString &tab)
{
    m_tab = tab;
}

QString ConfElement::getInterfaces() const
{
    return m_interfaces;
}

void ConfElement::setInterfaces(const QString &interfaces)
{
    m_interfaces = interfaces;
}

QString ConfElement::getEditClass() const
{
    return m_editClass;
}

void ConfElement::setEditClass(const QString &editClass)
{
    m_editClass = editClass;
}

void ConfElement::addInheritElements(Package *pack)
{
    for (const QString &name : m_inherit) {
        SharedConfElement e = pack->getElementByName(name);
        if (!e.isNull())
            m_inheritList.append(e);
    }
}

PropConfList ConfElement::getProperties() const
{
    return m_propList;
}

PointConfList ConfElement::getPoints() const
{
    return m_pointList;
}

PointConfList ConfElement::getHiddenPoints() const
{
    return m_hiddenPointList;
}

ConfElement::ConfElement(const QString &pathConf)
    : m_pathConf(pathConf)
{
    initConfElement();
    loadConf();
}

void ConfElement::initConfElement()
{
    QFileInfo file(m_pathConf);
    m_name = file.baseName();
}

void ConfElement::loadConf()
{
    QFile file(m_pathConf);
    file.open(QIODevice::ReadOnly);
    const QString data = QString::fromLocal8Bit(file.readAll());
    file.close();

    TypeSection section = ts_undefine;
    QStringList secAbouts;
    QStringList secTypes;
    QStringList secProperties;
    QStringList secPoints;

    for (QString line : data.split("\r\n")) {
        line = line.trimmed();
        int size = line.size();
        if (size < 2)
            continue;

        if (line.at(0) == QLatin1Char('[')) {
            const QChar c = line.at(1).toLower();

            if (c == QLatin1Char('a')) {
                section = ts_abouts;
            } else if (c == QLatin1Char('t')) {
                section = ts_types;
            } else if (c == QLatin1Char('e')) {
                section = ts_edit;
            } else if (c == QLatin1Char('p')) {
                section = ts_properties;
            } else if (c == QLatin1Char('m')) {
                section = ts_points;
            } else {
                section = ts_undefine;
            }

            continue;
        }

        switch (section) {
        case ts_abouts:
            secAbouts << line;
            break;
        case ts_types:
            secTypes << line;
            break;
        case ts_edit: {
            if (!m_editClass.isEmpty())
                continue;
            if (line.section('=', 0, 0).toLower() == QLatin1String("class"))
                m_editClass = line.section('=', 1, 1);
            break;
        }
        case ts_properties:
            secProperties << line;
            break;
        case ts_points:
            secPoints << line;
            break;
        default: {
            qWarning(qUtf8Printable("При загрузке конфигурации элемента, найден неизвестный параметр."));
            qWarning("Файл: \"%s\"", qUtf8Printable(m_pathConf));
            qWarning("Параметр: \"%s\"", qUtf8Printable(line));
            break;
        }
        }
    }

    parseAbout(secAbouts);
    parseTypes(secTypes);
    parseProperties(secProperties);
    parsePoints(secPoints);
}

void ConfElement::parseAbout(const QStringList &list)
{
    for (const QString &line : list) {
        const QString &name = line.section(QLatin1Char('='), 0, 0).toLower();
        const QString &value = line.section(QLatin1Char('='), 1, 1).toLower();

        if (name == QLatin1String("version")) {
            m_version = value;

        } else if (name == QLatin1String("author")) {
            m_author = value;

        } else if (name == QLatin1String("mail")) {
            m_mail = value;
        }
    }
}

void ConfElement::parseTypes(const QStringList &list)
{
    for (const QString &line : list) {
        QString sec0 = line.section("=", 0, 0).toLower();
        QString sec1 = line.section("=", 1, 1);
        if (sec0 == QLatin1String("class")) {
            m_class = ElementClassString.value(sec1);
        } else if (sec0 == QLatin1String("inherit")) {
            m_inherit = sec1.split(QLatin1Char(','), QString::SkipEmptyParts);
        } else if (sec0 == QLatin1String("sub")) {
            m_sub = sec1;
        } else if (sec0 == QLatin1String("info")) {
            m_info = sec1;
        } else if (sec0 == QLatin1String("icon")) {
            m_propIcon = sec1;
        } else if (sec0 == QLatin1String("view")) {
            m_view = sec1;
        } else if (sec0 == QLatin1String("tab")) {
            m_tab = sec1;
        } else if (sec0 == QLatin1String("interfaces")) {
            m_interfaces = sec1;
        }
    }
}

void ConfElement::parsePropValue(const QString &sline, SharedPropConf conf)
{
    const QString notImplemented = QString("Загрузка свойств с типом %1 не реализована.");
    auto &value = conf->value;

    if (sline.isEmpty())
        return;

    QStringList list = sline.split(QLatin1Char('|'), QString::SkipEmptyParts);
    if (list.isEmpty())
        return;

    const auto type = DataType(list[0].toInt());
    conf->value.setType(type);
    list.removeFirst();

    switch (type) {
    case data_null:
        break;
    case data_int: {
        if (!list.isEmpty())
            conf->value.setValue(list.first().toInt());
        else
            conf->value.setValue(0);
        break;
    }
    case data_real: {
        double dr = 0.0;
        if (!list.isEmpty())
            dr = list.first().toDouble();
        conf->value.setValue(dr);
        break;
    }
    case data_str: {
        QString ds;
        if (!list.isEmpty())
            ds = list.first();
        conf->value.setValue(ds);
        break;
    }
    case data_combo:
    case data_comboEx: {
        if (list.size() < 2) {
            qWarning() << "К-во параметров свойства меньше двух.";
            break;
        }
        conf->defLine = list.first().toInt();
        conf->value.setValue(list.last().split(','));
        break;
    }

    case data_color: {
        QString ds;
        if (!list.isEmpty())
            ds = list.first();
        conf->value.setValue(ds);
        break;
    }

    case data_font: {
        auto font = SharedValueFont::create();
        if (list.isEmpty()) {
            font->name = "MS Sans Serif";
            font->size = 8;
            font->color = 0;
            font->style = 0;
            font->charset = 1;

        } else {
            QStringList fontParams = list[0].split(QLatin1Char(','));
            if (fontParams.count() >= 5) {
                font->name = fontParams[0].trimmed();
                font->size = fontParams[1].toInt();
                font->color = fontParams[2].toInt();
                font->style = static_cast<uchar>(fontParams[3].toInt());
                font->charset = static_cast<uchar>(fontParams[4].toInt());
            }
        }

        value.setValue(font);
        break;
    }
    case data_flags:
        if (list.size() < 2) {
            qWarning() << "К-во параметров свойства меньше двух.";
            break;
        }

        conf->defLine = list[0].toInt();
        conf->value.setValue(list[1].split(QLatin1Char(',')));
        break;
    case data_jpeg:
    case data_icon:
    case data_bitmap:
    case data_stream:
    case data_wave:
    case data_array:
    case data_list:
    case data_data:
        break;

    case data_script:
    case data_element:
        //qInfo() << "test";
        //break;
    case data_matr:

    case data_menu:

    case data_code:

    case data_object:
        qWarning().noquote() << notImplemented.arg(DataTypeMap[type]);
        //qWarning().noquote() << notImplemented.arg(DataTypeMap[_propType]);
    }
}

void ConfElement::parseProperties(const QStringList &list)
{
    //##имя_группы=описание
    //## закрываем группу

    //+ activated - свойсво по-умолчанию, т.е. открывается редактор
    //при двойном клике.

    //@ makePoint - Позволяет активировать свойство в виде метода с префиксом do*

    //PropGroup
    QString nameGroup;

    //Parser
    bool beginGroup = false;

    for (const QString &line : list) {
        //ConfProp
        QString name;
        QString desc;
        SharedPropConf prop = SharedPropConf::create();

        //PropGroup
        QString descGroup;

        //Parser
        bool equalSign = false;
        bool beginGroupLine = false;
        uchar countSharp = 0;

        int outIndex = line.size();
        for (int i = 0; i <= outIndex; ++i) {
            if (i == outIndex) {
                if (beginGroupLine) {
                    m_group.insert(nameGroup, descGroup);
                    continue;
                }
                if (countSharp == 2) {
                    if (beginGroup) {
                        beginGroup = false;
                        nameGroup.clear();
                        continue;
                    }
                }
                break;
            }
            const QChar &c = line[i];

            if (i < 2) {
                if (c == QLatin1Char('#')) {
                    ++countSharp;
                    continue;
                } else if (c == QLatin1Char('@')) {
                    prop->makePoint = true;
                    continue;
                } else if (c == QLatin1Char('+')) {
                    prop->activated = true;
                    continue;
                }
            }

            if (c == QLatin1Char('=')) {
                equalSign = true;
                continue;
            }

            if (countSharp == 2) {
                if (!beginGroup) {
                    beginGroup = true;
                    beginGroupLine = true;
                }
            }

            if (beginGroupLine) {
                if (!equalSign)
                    nameGroup += c; //Имя группы
                else
                    descGroup += c; //Описание

                continue;
            }

            if (!equalSign)
                name += c; //Имя свойства
            else
                desc += c; //Описание свойства

            if (c == QLatin1Char('|')) {
                parsePropValue(line.right(outIndex - i - 1), prop);
                break;
            }

            prop->name = name;
            prop->desc = desc;
            if (beginGroup)
                prop->group = nameGroup;
            m_propList << prop;
        }
    }
}

void ConfElement::parsePoints(const QStringList &list)
{
    //* - скрытая точка
    for (const QString &line : list) {
        //ConfPoint
        QString name;
        QString desc;
        QString dataType;
        QString pointType;
        bool hidden = false;

        //Parser
        bool propBegin = false;
        bool propEnd = false;
        bool equalSign = false;
        uchar countPipe = 0;

        const int outIndex = line.size();
        for (int i = 0; i <= outIndex; ++i) {
            if (i == outIndex) {
                SharedPointConf point = SharedPointConf::create();
                point->name = name;
                point->desc = desc;
                point->pointType = PointType(pointType.toInt());
                point->dataType = DataType(dataType.toInt());

                if (hidden)
                    m_hiddenPointList << point;
                else
                    m_pointList << point;

                break;
            }
            const QChar &c = line[i];

            if (i == 0 && c == QLatin1Char('*')) {
                hidden = true;
                continue;
            }
            if (c == QLatin1Char('%')) {
                if (!propBegin)
                    propBegin = true;
                else
                    propEnd = true;

                name += c;
                continue;
            }
            if (c == QLatin1Char('=')) {
                equalSign = true;
                continue;
            }
            if (c == QLatin1Char('|')) {
                ++countPipe;
                continue;
            }
            if (propBegin && !propEnd) {
                name += c;
                continue;
            }
            if (countPipe) {
                if (countPipe == 1) //Тип точки
                    pointType += c;
                if (countPipe == 2) //Тип данных
                    dataType += c;

                continue;
            }
            if (!equalSign)
                name += c; //Имя точки
            else
                desc += c; //Описание точки
        }
    }
}
