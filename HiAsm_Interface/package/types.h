#pragma once

//Project
#include "cgt/CGTShare.h"
#include "scenemodel/value.h"

//STL

//Native

//Qt
#include "QtCore"
#include <QSharedPointer>

struct PointConf {
    /// Название точки.
    /// %prop% - название свойства, значение которого будет использоваться в имени точки.
    QString name;

    /// Описание точки.
    QString desc;

    /// Тип точки.
    PointType pointType;

    /// Тип данных точки.
    DataType dataType;
};
typedef QSharedPointer<PointConf> SharedPointConf;
typedef QList<SharedPointConf> PointConfList;

struct PropConf { //-V802
    QString name;
    QString desc;
    Value value;
    QStringList valueList;
    QString group;
    DataType type{};
    bool makePoint = false;
    bool activated = false;
};
typedef QSharedPointer<PropConf> SharedPropConf;
typedef QList<SharedPropConf> PropConfList;

typedef QMap<QString, QString> MapPropGroup;

struct ProjectInfo {
    QString name;
    QString shortDescription;
    QString description;
    QString entryElement;
    QString ext;
    QString make;
};
typedef QSharedPointer<ProjectInfo> SharedProjectInfo;
typedef QList<SharedProjectInfo> ProjectList;

struct CompilerInfo {
    QString name;
    QString path;
    QString command_line;
    QString ext;
};
typedef QSharedPointer<CompilerInfo> SharedCompilerInfo;
typedef QList<SharedCompilerInfo> CompilerList;

class Package;
typedef QMap<QString, Package *> Packages;
