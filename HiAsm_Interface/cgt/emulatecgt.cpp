//Project
#include "emulatecgt.h"
#include "cgt.h"
#include "scenemodel/container.h"
#include "scenemodel/element.h"
#include "scenemodel/point.h"
#include "scenemodel/property.h"
#include "scenemodel/valuetypes.h"

//STL

//Qt

#define EXPORT __stdcall

//! Храним указатель на модель данных
static SceneModel *m_model = nullptr;

//~~~~~~~~~~~~~~~~~ Проксированные функции ~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~ контейнер ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает количество элементов в контейнере.
static int EXPORT sdkGetCount(Container *container)
{
    return container->getCountElements();
}

//! Возвращает указатель на элемент по его Z-положению(индексу) в контейнере.
static Element *EXPORT sdkGetElement(Container *container, qint32 index)
{
    return container->getElementByIndex(index);
}

//! Возвращает указатель на элемент по имени элемента.
static Element *EXPORT sdkGetElementName(Container *container, const char *name)
{
    return container->getElementByName(QString::fromLocal8Bit(name));
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ элемент ~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Возвращает флаги элемента.
static ElementFlags EXPORT elGetFlag(Element *element)
{
    return element->getFlags();
}

//! Возвращает количество свойств элемента.
static qint32 EXPORT elGetPropCount(Element *element)
{
    return element->getCountProps();
}

//! Возвращает указатель на свойство элемента по индексу, с порядковым номером из INI.
static Property *EXPORT elGetProperty(Element *element, qint32 index)
{
    return element->getPropertyByIndex(index);
}

//! Возвращает True, если значение свойства совпадает с заданным в INI файле, иначе False.
static bool EXPORT elIsDefProp(Element *element, qint32 index)
{
    Property *p = element->getPropertyByIndex(index);
    if (!p)
        return false;

    return p->getIsDefProp();
}

//! Присваиваем элементу уникальное имя и возвращаем указатель на этот элемент.
static Element *EXPORT elSetCodeName(Element *element, const char *name)
{
    element->setCodeName(QString::fromLocal8Bit(name));
    return element;
}

//! Возвращает уникальное имя элемента.
static const char *EXPORT elGetCodeName(Element *element)
{
    return fcgt::strToCString(element->getCodeName());
}

//! Возвращает имя класса элемента.
static const char *EXPORT elGetClassName(Element *element)
{
    return fcgt::strToCString(element->getClassName());
}

//! Возвращает содержимое поля Sub из конфигурационного INI-файла элемента.
static const char *EXPORT elGetInfSub(Element *element)
{
    return fcgt::strToCString(element->getInfSub());
}

//! Возвращает общее количество видимых точек у элемента.
static int EXPORT elGetPtCount(Element *element)
{
    return element->getCountPoints();
}

//! Возвращает указатель на точку по её индексу.
static Point *EXPORT elGetPt(Element *element, int index)
{
    return element->getPointByIndex(index);
}

//! Возвращает указатель на точку по её имени.
static Point *EXPORT elGetPtName(Element *element, const char *name)
{
    return element->getPointByName(QString::fromLocal8Bit(name));
}

//! Возвращает индекс класса элемента.
static ElementClass EXPORT elGetClassIndex(Element *element)
{
    return element->getClassIndex();
}

//! Получаем указатель на контейнер из элемента.
//! Если элемент не содержит контейнер,
//! возвращаем указатель на родительский элемент контейнера целевого элемента.
static QObject *EXPORT elGetSDK(Element *element)
{
    Container *c = element->getContainer();
    if (c)
        return c;

    Element *e = qobject_cast<Element *>(element->parent()->parent());
    if (e)
        return e;

    return nullptr;
}

//! Возвращает True, если данный элемент является ссылкой, либо на него ссылаются.
static bool EXPORT elLinkIs(Element *element)
{
    return element->getLinkIs();
}

//! Возвращает указатель на главный элемент(тот, на который ссылаются другие).
static Element *EXPORT elLinkMain(Element *element)
{
    return element->getLinkMain();
}

//! Помещает в переменные "X" и "Y", позицию элемента в редакторе схем.
static void EXPORT elGetPos(Element *element, int &X, int &Y)
{
    X = element->getPosX();
    Y = element->getPosY();
}

//! Помещает в переменные "W" и "H", размеры элемента.
static void EXPORT elGetSize(Element *element, int &W, int &H)
{
    W = element->getSizeW();
    H = element->getSizeH();
}

//! Возвращает ID элемента.
//[deprecated]
static qint32 EXPORT elGetEID(Element *element)
{
    return element->getId();
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ точки элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Возвращает указатель на точку с которой соединена указанная.
static Point *EXPORT ptGetLinkPoint(Point *point)
{
    return point->getLinkPoint();
}

//! Возвращает указатель на точку с которой соединена указанная,
//! без учета точек разрыва и хабов.
static Point *EXPORT ptGetRLinkPoint(Point *point)
{
    return point->getRLinkPoint();
}

//! Получаем тип точки.
static PointType EXPORT ptGetType(Point *point)
{
    return point->getType();
}

//! Возвращает имя точки.
static const char *EXPORT ptGetName(Point *point)
{
    return fcgt::strToCString(point->getName());
}

//! Возвращает указатель на родителя точки (элемент).
static Element *EXPORT ptGetParent(Point *point)
{
    return qobject_cast<Element *>(point->parent());
}

//! Получаем индекс точки относительно точек того же типа.
static int EXPORT ptGetIndex(Point *point)
{
    return point->getIndex();
}

//! Возвращает базовую часть имени динамической точки(для CI_DPElement).
static const char *EXPORT pt_dpeGetName(Point *point)
{
    return fcgt::strToCString(point->getDpeName());
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ свойства элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Возвращает тип свойства.
static DataType EXPORT propGetType(Property *prop)
{
    return prop->getType();
}

//! Возвращает имя свойства.
static const char *EXPORT propGetName(Property *prop)
{
    return fcgt::strToCString(prop->getName());
}

//! Возвращает значение свойства в виде указателя на данные.
static const Value *EXPORT propGetValue(Property *prop)
{
    return prop->getPtrValue();
}

//! Возвращает значение свойства в формате uchar.
static uchar EXPORT propToByte(Property *prop)
{
    return prop->toByte();
}

//! Возвращает значение свойства в формате int.
static int EXPORT propToInteger(Property *prop)
{
    return prop->toInt();
}

//! Возвращает значение свойства в формате qreal.
static qreal EXPORT propToReal(Property *prop)
{
    return prop->toReal();
}

//! Возвращает значение свойства в виде C строки.
static const char *EXPORT propToString(Property *prop)
{
    return fcgt::strToCString(prop->toString());
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ ресурсы ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Добавляет имя файла в список временных файлов.
//! При завершении работы с классом SceneModel, все файлы из списка должны быть удалены.
static qint32 EXPORT resAddFile(const char *filePath)
{
    return m_model->addResList(QString::fromLocal8Bit(filePath));
}

//! Добавляет иконку в ресурсы и в список временных файлов.
//! Возвращает имя временного файла.
static const char *EXPORT resAddIcon(Property *prop)
{
    return m_model->addStreamRes(prop);
}

//! Добавляет строку в ресурсы и в список временных файлов.
//! Возвращает имя временного файла.
static const char *EXPORT resAddStr(const char *string)
{
    return m_model->addStringRes(QString::fromLocal8Bit(string));
}

//! Добавляет поток (данные) в ресурсы и в список временных файлов.
//! Возвращает имя временного файла.
//! Временный файл создаётся в папке %HiAsm%\compiler и существует до конца
//! работы с библиотекой.
static const char *EXPORT resAddStream(Property *prop)
{
    return m_model->addStreamRes(prop);
}

//! Добавляет звук в ресурсы и в список временных файлов.
//! Возвращает имя временного файла.
//! Временный файл создаётся в папке %HiAsm%\compiler и существует до конца
//! работы с библиотекой.
static const char *EXPORT resAddWave(Property *prop)
{
    return m_model->addStreamRes(prop);
}

//! Добавляет картинку в ресурсы и в список временных файлов.
//! Возвращает имя временного файла.
//! Временный файл создаётся в папке %HiAsm%\compiler и существует до конца
//! работы с библиотекой.
static const char *EXPORT resAddBitmap(Property *prop)
{
    return m_model->addStreamRes(prop);
}

//! Добавляет меню в ресурсы и в список временных файлов.
//[deprecated]
static const char *EXPORT resAddMenu(qint32 prop)
{
    Q_UNUSED(prop)
    return nullptr;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ информационные сообщения ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Выводит строку text в отладочный поток цветом color.
//! Всегда возвращает 0.
static int EXPORT _Debug(const char *text, qint32 color)
{
    Q_UNUSED(color)

    qDebug("%s", text);
    return 0;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ среда ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! На основе индекса параметра, записываем данные в память value
//! Всегда возвращает 0.
static qint32 EXPORT GetParam(CgtParams index, void *value)
{
    m_model->getCgtParam(index, value);
    return 0;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ массив ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Получаем количество элементов в массиве.
static int EXPORT arrCount(Value *array)
{
    return array->getArraySize();
}

//! Получаем тип элементов в массиве.
static DataType EXPORT arrType(Value *array)
{
    return array->getTypeArrayItem();
}

//! Получаем имя элемента по индексу.
static const char *EXPORT arrItemName(Value *array, int index)
{
    return fcgt::strToCString(array->getArrayItemName(index));
}

//! Получаем элемент (arrayValue) массива  по индексу.
//[deprecated]
static void *EXPORT arrItemData(Value *array, int index)
{
    Q_UNUSED(array)
    Q_UNUSED(index)
    return nullptr;
}

//! Получаем элемент массива в виде свойства (prop) Оо,
//! для дальнейшей работы с ним cgt::prop* функциями.
static Property *EXPORT arrGetItem(Value *array, int index)
{
    static Property prop;
    const SharedArrayItem &arrItem = array->getArrayItemByIndex(index);
    if (arrItem) {
        prop.setType(array->getTypeArrayItem());
        prop.setValue(arrItem->data);
        prop.setName(arrItem->name);

        return &prop;
    }

    return nullptr;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ среда ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает статус отладки запущенной схемы.
//! Если число больше нуля - отладка включена.
//! Если число равно нулю - отладка выключена.
static int EXPORT isDebug(Element *element)
{
    Q_UNUSED(element)
    return 0;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ работа с данными ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает тип данных.
static DataType EXPORT dtType(Value *data)
{
    return data->getType();
}

//! Возвращает данные в формате: строка в стиле C.
static const char *EXPORT dtStr(Value *data)
{
    return fcgt::strToCString(data->toString());
}

//! Возвращает данные в формате: int.
static int EXPORT dtInt(Value *data)
{
    return data->toInt();
}

//! Возвращает данные в формате: qreal.
static qreal EXPORT dtReal(Value *data)
{
    return data->toReal();
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ шрифт ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает название шрифта.
static const char *EXPORT fntName(Value *valueFont)
{
    const SharedValueFont &font = valueFont->toFont();
    if (!font)
        return nullptr;

    return fcgt::strToCString(font->name);
}
//! Возвращает размер шрифта.
static int EXPORT fntSize(Value *valueFont)
{
    const SharedValueFont &font = valueFont->toFont();
    if (!font)
        return 0;

    return font->size;
}
//! Возвращает стиль шрифта.
static uchar EXPORT fntStyle(Value *valueFont)
{
    const SharedValueFont &font = valueFont->toFont();
    if (!font)
        return 0;

    return font->style;
}
//! Возвращает цвет шрифта.
static qint32 EXPORT fntColor(Value *valueFont)
{
    const SharedValueFont &font = valueFont->toFont();
    if (!font)
        return 0;

    return font->color;
}
//! Возвращает кодировку шрифта.
static uchar EXPORT fntCharSet(Value *valueFont)
{
    const SharedValueFont &font = valueFont->toFont();
    if (!font)
        return 0;

    return font->charset;
}

//!~~~~~~~~~~~~~~~~элемент | пользовательские данные ~~~~~~~~~~~~~
//!ru Судя по CodeGen.dpr, используется для хранения указателя (ID элемента) на самого себя.

//! Возвращает пользовательские данные элемента.
//! Коммент из hiasm5 - user data used in FTCG codegen.
//! Судя по всему, данные могут быть любого типа, ибо хранит указатель..
//! Так же, скорее всего используется для хранения ID элемента
static qint32 EXPORT elGetData(Element *element)
{
    return element->getUserData();
}

//! Устанавливает пользовательские данные элементу.
//! Коммент из hiasm5 - user data used in FTCG codegen.
//! Судя по всему, данные могут быть любого типа, ибо хранит указатель.
//! Так же, скорее всего используется для хранения ID элемента
static void EXPORT elSetData(Element *element, qint32 data)
{
    element->setUserData(data);
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ точки элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает тип данных точки.
static DataType EXPORT ptGetDataType(Point *point)
{
    return point->getDataType();
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ элемент ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает указатель на родительский контейнер элемента.
static Container *EXPORT elGetParent(Element *element)
{
    return element->getParent();
}

//! Возвращает количество свойств в списке свойств(из панели свойств).
//[deprecated]
static int EXPORT elGetPropertyListCount(Element *element)
{
    Q_UNUSED(element)

    return 0;
}

//! Возвращает свойство из списка свойств (PropertyList).
//[deprecated]
static int EXPORT elGetPropertyListItem(Element *element, int index)
{
    Q_UNUSED(element)
    Q_UNUSED(index)

    return 0;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ список свойств элемента ~~~~~~~~~~~~~~~~~~~~~~~~
//! Возвращает имя свойства.
//[deprecated]
static const char *EXPORT plGetName(int prop)
{
    Q_UNUSED(prop)
    return nullptr;
}

//! Возвращает описание свойства.
//[deprecated]
static const char *EXPORT plGetInfo(int prop)
{
    Q_UNUSED(prop)
    return nullptr;
}

//! Возвращает группу свойсва.
//[deprecated]
static const char *EXPORT plGetGroup(int prop)
{
    Q_UNUSED(prop)
    return nullptr;
}

//! Возвращает указатель на данные свойства.
//[deprecated]
static qint32 EXPORT plGetProperty(int prop)
{
    Q_UNUSED(prop)
    return 0;
}

//! Возвращает ID родительского элемента указанного свойства.
//[deprecated]
static qint32 EXPORT plGetOwner(int prop)
{
    Q_UNUSED(prop)
    return 0;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ точки элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Возвращает описание точки.
static const char *EXPORT ptGetInfo(Point *point)
{
    return fcgt::strToCString(point->getInfo());
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ свойства элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает указатель на элемент, который прилинкован к указанному свойству.
static Element *EXPORT propGetLinkedElement(Element *element, const char *propName)
{
    Property *p = element->getPropertyByName(QString::fromLocal8Bit(propName));
    if (!p)
        return nullptr;

    const SharedLinkedElementInfo info = p->toLinkedElementInfo();
    if (!info)
        return nullptr;

    return info->element;
}

//! Возвращает 1, если свойство помечено на перевод.
//[deprecated]
static int EXPORT propIsTranslate(Element *element, Property *prop)
{
    Q_UNUSED(element)
    Q_UNUSED(prop)
    return 0;
}

/*!
 * Возвращает указатель на элемент, прилинкованного к указанному свойству.
 * В буфер buf пишется имя интерфейса элемента.
 * Например в строке из INI: FormFastening=Форма для привязки позиции|20|(empty)|ControlManager
 * ControlManager - является той самой информацией передаваемой нам в buf.
 */
static Element *EXPORT propGetLinkedElementInfo(Element *element, Property *prop, char *buf)
{
    Q_UNUSED(element)
    const SharedLinkedElementInfo &info = prop->toLinkedElementInfo();
    if (!info)
        return 0;

    strcpy(buf, info->interface.toStdString().c_str());
    return info->element;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ элемент - CI_PolyMulti ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает указатель на контейнер по его индексу из элемента.
static Container *EXPORT elGetSDKByIndex(Element *element, int index)
{
    return element->getContainerByIndex(index);
}

//! Возвращает количество контейнеров полиморфного элемента(CI_PolyMulti).
static int EXPORT elGetSDKCount(Element *element)
{
    return element->getCountContainers();
}

//! Возвращает имя контейнера по индексу.
static const char *EXPORT elGetSDKName(Element *element, int index)
{
    const Container *c = element->getContainerByIndex(index);
    if (!c)
        return nullptr;

    return fcgt::strToCString(c->getName());
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ схема ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает указатель на элемент родитель для данного SDK.
//! Возвращает 0, если контейнер не имеет родителя.
static Element *EXPORT sdkGetParent(Container *container)
{
    return container->getParent();
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ элемент ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает интерфейсы, предоставляемые элементом.
//! Содержимое поля Interfaces= из конфигурации элемента.
static const char *EXPORT elGetInterface(Element *element)
{
    return fcgt::strToCString(element->getInterfaces());
}

//! Возвращает список классов, от которых наследуется элемент
//! Содержимое поля Inherit= из конфигурации элемента.
static const char *EXPORT elGetInherit(Element *element)
{
    return fcgt::strToCString(element->getInherit());
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ ресурсы ~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Возвращает 1, если список ресурсов пуст, и 0 в противном случае.
static int EXPORT resEmpty()
{
    return m_model->resIsEmpty();
}

//! Устанавливает префикс для имен ресурсов.
//[deprecated]
static qint32 EXPORT resSetPref(const char *pref)
{
    Q_UNUSED(pref)
    return 0;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ информационные сообщения ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Добавляет информацию в информационную панель
static int EXPORT _Error(int line, Element *element, const char *text)
{
    Q_UNUSED(line)
    Q_UNUSED(element)
    Q_UNUSED(text)
    return 0;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ элемент ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Возвращает ID группы, к которой принадлежит элемент и 0, если группа отсутствует
//[deprecated]
static int EXPORT elGetGroup(Element *element)
{
    Q_UNUSED(element)
    return 0;
}

//!~~~~~~~~~~~~~~~~~~~~~~~~ свойства элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~

//! Сохраняет данные свойства в файл.
//[deprecated]
static int EXPORT propSaveToFile(Property *prop, const char *fileName)
{
    Q_UNUSED(prop)
    Q_UNUSED(fileName)

    return 0;
}

//Заполняем массив указателей
static void *emulateCgt[]{
    reinterpret_cast<void *>(sdkGetCount),
    reinterpret_cast<void *>(sdkGetElement),
    reinterpret_cast<void *>(sdkGetElementName),
    reinterpret_cast<void *>(elGetFlag),
    reinterpret_cast<void *>(elGetPropCount),
    reinterpret_cast<void *>(elGetProperty),
    reinterpret_cast<void *>(elIsDefProp),
    reinterpret_cast<void *>(elSetCodeName),
    reinterpret_cast<void *>(elGetCodeName),
    reinterpret_cast<void *>(elGetClassName),
    reinterpret_cast<void *>(elGetInfSub),
    reinterpret_cast<void *>(elGetPtCount),
    reinterpret_cast<void *>(elGetPt),
    reinterpret_cast<void *>(elGetPtName),
    reinterpret_cast<void *>(elGetClassIndex),
    reinterpret_cast<void *>(elGetSDK),
    reinterpret_cast<void *>(elLinkIs),
    reinterpret_cast<void *>(elLinkMain),
    reinterpret_cast<void *>(elGetPos),
    reinterpret_cast<void *>(elGetSize),
    reinterpret_cast<void *>(elGetEID),
    reinterpret_cast<void *>(ptGetLinkPoint),
    reinterpret_cast<void *>(ptGetRLinkPoint),
    reinterpret_cast<void *>(ptGetType),
    reinterpret_cast<void *>(ptGetName),
    reinterpret_cast<void *>(ptGetParent),
    reinterpret_cast<void *>(ptGetIndex),
    reinterpret_cast<void *>(pt_dpeGetName),
    reinterpret_cast<void *>(propGetType),
    reinterpret_cast<void *>(propGetName),
    reinterpret_cast<void *>(propGetValue),
    reinterpret_cast<void *>(propToByte),
    reinterpret_cast<void *>(propToInteger),
    reinterpret_cast<void *>(propToReal),
    reinterpret_cast<void *>(propToString),
    reinterpret_cast<void *>(resAddFile),
    reinterpret_cast<void *>(resAddIcon),
    reinterpret_cast<void *>(resAddStr),
    reinterpret_cast<void *>(resAddStream),
    reinterpret_cast<void *>(resAddWave),
    reinterpret_cast<void *>(resAddBitmap),
    reinterpret_cast<void *>(resAddMenu),
    reinterpret_cast<void *>(_Debug),
    reinterpret_cast<void *>(GetParam),
    reinterpret_cast<void *>(arrCount),
    reinterpret_cast<void *>(arrType),
    reinterpret_cast<void *>(arrItemName),
    reinterpret_cast<void *>(arrItemData),
    reinterpret_cast<void *>(arrGetItem),
    reinterpret_cast<void *>(isDebug),
    reinterpret_cast<void *>(dtType),
    reinterpret_cast<void *>(dtStr),
    reinterpret_cast<void *>(dtInt),
    reinterpret_cast<void *>(dtReal),
    reinterpret_cast<void *>(fntName),
    reinterpret_cast<void *>(fntSize),
    reinterpret_cast<void *>(fntStyle),
    reinterpret_cast<void *>(fntColor),
    reinterpret_cast<void *>(fntCharSet),
    reinterpret_cast<void *>(elGetData),
    reinterpret_cast<void *>(elSetData),
    reinterpret_cast<void *>(ptGetDataType),
    reinterpret_cast<void *>(elGetParent),
    reinterpret_cast<void *>(elGetPropertyListCount),
    reinterpret_cast<void *>(elGetPropertyListItem),
    reinterpret_cast<void *>(plGetName),
    reinterpret_cast<void *>(plGetInfo),
    reinterpret_cast<void *>(plGetGroup),
    reinterpret_cast<void *>(plGetProperty),
    reinterpret_cast<void *>(plGetOwner),
    reinterpret_cast<void *>(ptGetInfo),
    reinterpret_cast<void *>(propGetLinkedElement),
    reinterpret_cast<void *>(propIsTranslate),
    reinterpret_cast<void *>(propGetLinkedElementInfo),
    reinterpret_cast<void *>(elGetSDKByIndex),
    reinterpret_cast<void *>(elGetSDKCount),
    reinterpret_cast<void *>(elGetSDKName),
    reinterpret_cast<void *>(sdkGetParent),
    reinterpret_cast<void *>(elGetInterface),
    reinterpret_cast<void *>(elGetInherit),
    reinterpret_cast<void *>(resEmpty),
    reinterpret_cast<void *>(resSetPref),
    reinterpret_cast<void *>(_Error),
    reinterpret_cast<void *>(elGetGroup),
    reinterpret_cast<void *>(propSaveToFile),
};

/*!  Служебные функции   */
//Сохранение указателя для дальнейшей работы с оным
void EmulateCgt::setSceneModel(SceneModel *model)
{
    m_model = model;
}

//Получаем массив указателей на функции
TCodeGenTools *EmulateCgt::getCgt()
{
    return reinterpret_cast<TCodeGenTools *>(emulateCgt);
}
