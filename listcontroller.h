#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include "object.h"
#include <QMap>

enum ValType {
    ERROR,
    PATH,
    UINT,
    REAL,
    ENUM
};

class ListController
{
    const QString m_propertiesPath;
    QMap <QString, QStringList> m_properties;  //<ObjectName, List of Property "PropertyName : value">
    QVector <Object> m_vec;

    bool readProperty();

public:
    ListController();
    bool readObjectsList(const QString& path);
    bool saveObjectList(const QString& path);
    QVector <Object>& getObjects();
    QStringList getObjectsName() const;
    ValType getType(const QString& name, const QString& property, QString* description);

};

#endif // LISTCONTROLLER_H
