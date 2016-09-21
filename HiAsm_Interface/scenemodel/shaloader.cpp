//Project
#include "shaloader.h"
#include "container.h"
#include "element.h"

//STL

//Qt
#include <QDebug>
#include <QFile>
#include <QRegExp>
#include <QStringList>
#include <QtCore>

SHALoader::SHALoader(const QString &filePath, SceneModel *model, QObject *parent)
    : QObject(parent)
    , m_filePath(filePath)
    , m_model(model)
{

}

SHALoader::SHALoader(SceneModel *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
{
}

bool SHALoader::loadSha()
{
    m_content.clear();
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    const QStringList &tmpContent = QString::fromLocal8Bit(file.readAll()).split("\r\n");
    for (const QString &s : tmpContent) {
        const QString tmp = s.trimmed();
        if (!tmp.isEmpty())
            m_content << tmp;
    }

    if (!parseHeader())
        return false;

    Container *c = parseElements();
    if (!c)
        return false;

    m_model->setRootContainer(c);
    return true;
}

QString SHALoader::getFilePath() const
{
    return m_filePath;
}

void SHALoader::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

QString SHALoader::findBlock(const QString &line, const QString &beginTok,
                             const QString &endTok)
{
    const qint32 beginTokLen = beginTok.length();
    const auto begin = line.indexOf(beginTok);
    const auto end = line.lastIndexOf(endTok);
    const qint32 count = end - begin;
    return line.mid(begin + beginTokLen, count - beginTokLen);
}

QStringList SHALoader::findMultiBlock(QString &str, const QString &beginTok,
                                      const QString &endTok, bool cutBlock,
                                      bool removeTok)
{
    qint32 index = 0;
    QStringList list;
    if (str.isEmpty())
        return list;

    while (true) {
        int beginTokLen = beginTok.length();
        int endTokLen = endTok.length();
        auto begin = str.indexOf(beginTok, index);
        if (begin == -1)
            break;

        auto end = str.indexOf(endTok, begin);
        if (end == -1)
            break;

        qint32 count = end - begin;
        list << str.mid(begin + beginTokLen, count - beginTokLen);
        if (cutBlock) {
            if (removeTok) {
                str = str.remove(begin, count + beginTokLen);
                index = end - count;
            } else {
                str = str.remove(begin + beginTokLen, count - beginTokLen);
                index = end - count + beginTokLen + endTokLen;
            }
        } else {
            index = end + endTokLen;
        }
    }

    return list;
}

QPair<QString, QString> SHALoader::splitSLine(const QString &sline, const QChar &del, ParseType type)
{
    QString first;
    QString second;

    bool split = false;
    auto delimiter = [&](int idx) {
        const QChar c = sline.at(idx);
        if ((split == false) && (c == del)) {
            split = true;
            return;
        }

        if (!split) {
            first += c;
        } else {
            second += c;
        }
    };

    int size = sline.size();
    if (type == BeginToEnd) {
        for (int i = 0; i < size; ++i) {
            delimiter(i);
        }
    } else {
        for (int i = size; i > 0; --i) {
            delimiter(i);
        }
    }

    return {first, second};
}

QVariantMap SHALoader::linkToVariantMap(const QString &sline)
{
    QVariantMap map;
    QStringList res;
    const QString link = findBlock(sline, "link(", ")");
    const QStringList blockList = link.split(':');
    if (blockList.size() < 2)
        return QVariantMap();

    // Block1
    const QStringList block1List = blockList.at(0).split(',');
    if (block1List.size() < 2)
        return QVariantMap();

    // Block2
    QString block2 = blockList.at(1);
    const QStringList nodes = findMultiBlock(block2, "(", ")", true, true);
    const QStringList block2list = block2.split(',');
    if (block2list.empty())
        return QVariantMap();

    map.insert("thisPoint", block1List.at(0));
    map.insert("targetId", block1List.at(1).toInt());
    map.insert("targetPoint", block2list.at(0));

    if (!nodes.isEmpty())
        map.insert("nodes", nodes);

    return map;
}

QVariantMap SHALoader::propToVariantMap(const QString &sline)
{
    if (sline.isEmpty())
        return QVariantMap();

    auto pairBlock = splitSLine(sline, '=');
    QString first = pairBlock.first;
    QString second = pairBlock.second;

    bool isHide = false;
    //Скрытое свойство
    if (first.at(0) == QLatin1Char('@')) {
        first.remove(0, 1);
        isHide = true;
    }

    return QVariantMap();
}

SHALoader::LineType SHALoader::getLineType(const QString &sline)
{
    auto checkPattern = [&sline](const QString &pattern) {
        return QRegExp(pattern, Qt::CaseSensitive, QRegExp::WildcardUnix)
            .exactMatch(sline);
    };

    // Ignore
    if (checkPattern("AddHint(*"))
        return LineType::Ignore;
    if (checkPattern("\\**"))
        return LineType::Ignore;
    if (checkPattern("Pos(*)"))
        return LineType::Ignore;
    if (checkPattern("elink(*)"))
        return LineType::Ignore;
    if (checkPattern("MakeExt(*)"))
        return LineType::Ignore;
    if (checkPattern("PColor(*)"))
        return LineType::Ignore;
    if (checkPattern("MakeTrans(*)"))
        return LineType::Ignore;

    //Актуальные
    if (checkPattern("Make(*)"))
        return LineType::Make;
    if (checkPattern("ver(*)"))
        return LineType::Ver;
    if (checkPattern("Add(*)"))
        return LineType::Add;
    if (checkPattern("{"))
        return LineType::OpenBlock;
    if (checkPattern("}"))
        return LineType::CloseBlock;
    if (checkPattern("link(*)"))
        return LineType::Link;
    if (checkPattern("Point(*)"))
        return LineType::Point;
    if (checkPattern("@*=*"))
        return LineType::HideProp;
    if (checkPattern("*=*"))
        return LineType::Prop;
    if (checkPattern("BEGIN_SDK"))
        return LineType::BEGIN_SDK;
    if (checkPattern("END_SDK"))
        return LineType::END_SDK;
    if (sline.isEmpty())
        return LineType::Empty;

    return LineType::Undefined;
}

SHALoader::LineType SHALoader::getLineType(const QStringList &content, int idx)
{
    int size = content.size();
    if ((idx < 0) || (idx > (size - 1)))
        return LineType::Null;

    return getLineType(content[idx]);
}

bool SHALoader::parseHeader()
{
    bool state = false;
    for (const QString &line : m_content) {
        switch (getLineType(line)) {
        case LineType::Make: {
            const QString pack = findBlock(line, "Make(", ")");
            state = m_model->loadPackage(pack);

            continue;
        }
        case LineType::Ver:
            //header.insert("version", findBlock(line, "ver(", ")"));
            continue;
        case LineType::Add:
            return state;
        case LineType::Ignore:
        case SHALoader::Null:
        case SHALoader::Undefined:
        case SHALoader::OpenBlock:
        case SHALoader::CloseBlock:
        case SHALoader::Link:
        case SHALoader::Point:
        case SHALoader::HideProp:
        case SHALoader::Prop:
        case SHALoader::BEGIN_SDK:
        case SHALoader::END_SDK:
        case SHALoader::Empty:
            continue;
        }
    }

    return state;
}

Container *SHALoader::parseElements(int begin, int _size, int *prev)
{
    Container *container = new Container(m_model);
    Element *element = nullptr;
    //QVariantList linkList;
    //QVariantList pointList;
    //QVariantList propList;

    const int size = (_size <= 0) ? m_content.size() : _size;
    for (int i = begin; i < size; ++i) {

        const QString sline = m_content[i];
        const LineType type = getLineType(sline);

        switch (type) {
        case LineType::Add: {
            QStringList params = findBlock(sline, "Add(", ")").split(',');
            if (params.size() < 4) {
                emit onError("К-во аргументов меньше 4-х.");
                return nullptr;
            }

            //Основные параметры элемента
            QString name = params[0];
            int id = params[1].toInt();
            int x = params[2].toInt();
            int y = params[3].toInt();

            element = new Element(name, id, x, y, container);
            break;
        }
        case LineType::Link: {
            const QVariantMap link = linkToVariantMap(sline);
            if (link.isEmpty()) {
                emit onError("Ошибка при разборе параметров link(*)");
                return nullptr;
            }
            //linkList.append(link);
            continue;
        }
        case LineType::Point: {
            const QString point = findBlock(sline, "(", ")");
            if (!point.isEmpty()) {
                //element->addPoint(point);
            } else {
                emit onError("Ошибка при разборе параметров Point(*)");
                return nullptr;
            }

            continue;
        }
        case LineType::HideProp:
        case LineType::Prop: {
            const QVariantMap prop = propToVariantMap(sline);
            if (!prop.isEmpty()) {
                //propList << prop;
            } else {
                emit onError("Ошибка при разборе свойства");
                return nullptr;
            }

            continue;
        }
        case LineType::CloseBlock: {
            //Элемент является контейнером
            if (getLineType(m_content, i + 1) == LineType::BEGIN_SDK) {
                Container *c = parseElements(i + 2, size, &i);
                if (!c)
                    return nullptr;

                element->addContainer(c);
            }

            //if (!linkList.isEmpty())
            //    element.insert("links", linkList);
            //if (!pointList.isEmpty())
            //    element.insert("points", pointList);
            //if (!propList.isEmpty())
            //    element.insert("props", propList);

            //linkList.clear();
            //pointList.clear();
            //propList.clear();
            //container += element;
            container->addElement(element);
            element = nullptr;

            continue;
        }
        case LineType::END_SDK: {
            *prev = i;
            return container;
        }
        case SHALoader::Null:
        case SHALoader::Undefined:
        case SHALoader::Ignore:
        case SHALoader::Make:
        case SHALoader::Ver:
        case SHALoader::OpenBlock:
        case SHALoader::BEGIN_SDK:
        case SHALoader::Empty:
            continue;
        }
    }

    return container;
}
