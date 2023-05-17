#ifndef OBJECT_H
#define OBJECT_H

#include <QString>
#include <QVector>


class Object
{
    QVector <QPair<QString, QString>> m_property;

public:
    Object();
    QString getName() const;
    bool addProperty(const QString& line);
    bool addProperty(const QString& name, const QString& val);
    QVector <QPair<QString, QString>> getPropertyWithoutName() const;
    QVector <QPair<QString, QString>> getProperty() const;
};

#endif // OBJECT_H
