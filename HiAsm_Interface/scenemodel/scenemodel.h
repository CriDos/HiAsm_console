#pragma once

//Project
#include "cgt/CGTShare.h"
#include "package/packagemanager.h"
#include "property.h"

//STL

//Qt
#include <QObject>
#include <QSet>

class Container;
class Element;

class SceneModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SceneModel)

private:
    //Package
    static Package *m_package;
    PackageManager *m_packageManager{};

    //List
    static QMap<qint32, Element *> m_elementList;

    //Container
    Container *m_rootContainer{};

    //Resource
    const QString m_resourcesDir = "resources";
    QSet<QString> m_resourcesToDelete;
    QMap<QString, QString> m_resourcesForCompile;

    //Параметры CGT
    bool m_isDebug{};
    qint32 m_debugMode = false;
    qint32 m_debugServerPort = 120;
    qint32 m_debugClientPort = 121;
    qint32 m_sdeWidth = 0;
    qint32 m_sdeHeight = 0;
    QString m_hiasmVersion = "4.5.186";
    QString m_userName;
    QString m_userMail;
    QString m_codePath;
    QString m_projectPath;
    QString m_projectName;
    QString m_compiler;

public:
    explicit SceneModel(PackageManager *package, QObject *parent = 0);
    virtual ~SceneModel();

public:
    static int genId()
    {
        return 0;
    }

    //Model
    bool loadFromSha(const QString &filePath);

    //Package
    static Package *getPackage();
    bool loadPackage(const QString &name);

    //Elements
    static void addElementToList(Element *element);
    static Element *getElementFromEId(qint32 id_element);

    //Container
    Container *getRootContainer() const;
    void setRootContainer(Container *container);

    //Resource
    const char *addStreamRes(Property *id_prop);
    const char *addStringRes(const QString &str);
    void deleteResources();
    void compileResources();

    qint32 addResList(const QString &filePath);
    bool resIsEmpty() const;

    //Параметры CGT
    void getCgtParam(CgtParams index, void *buf) const;

    bool getIsDebug() const;
    void setIsDebug(bool isDebug);

    qint32 getDebugMode() const;
    void setDebugMode(qint32 debugMode);

    qint32 getDebugServerPort() const;
    void setDebugServerPort(qint32 debugServerPort);

    qint32 getDebugClientPort() const;
    void setDebugClientPort(qint32 debugClientPort);

    qint32 getSdeWidth() const;
    void setSdeWidth(qint32 sdeWidth);

    qint32 getSdeHeight() const;
    void setSdeHeight(qint32 sdeHeight);

    QString getCodePath() const;
    void setCodePath(const QString &codePath);

    QString getProjectPath() const;
    void setProjectPath(const QString &projectPath);

    QString getHiasmVersion() const;
    void setHiasmVersion(const QString &hiasmVersion);

    QString getUserName() const;
    void setUserName(const QString &userName);

    QString getUserMail() const;
    void setUserMail(const QString &userMail);

    QString getProjectName() const;
    void setProjectName(const QString &projectName);

    QString getCompiler() const;
    void setCompiler(const QString &compiler);
};

Q_DECLARE_METATYPE(SceneModel *)
