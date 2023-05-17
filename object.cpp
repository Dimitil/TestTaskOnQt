#include "object.h"

Object::Object()
{

}

QString Object::getName() const {
    for(const auto& pair : m_property) {
        if(pair.first == "Name") {
            return pair.second;
        }
    }
    return "";
}

bool Object::addProperty(const QString &line) {
    if(line.isEmpty()) {
        return false;
    }

    QStringList list = line.split(" ");
    if(list.size() != 2) {
        return false;
    }
    return addProperty(list[0], list[1]);
}

bool Object::addProperty(const QString &name, const QString &val) {
    for(auto& p : m_property) {
        if(p.first == name) {
            p.second = val;
            p.second.replace(",", "."); //separator in double
            return true;
        }
    }
    m_property.push_back(qMakePair(name, val));
    return true;
}

QVector<QPair<QString, QString> > Object::getPropertyWithoutName() const {
    QVector<QPair<QString, QString>> res;
    for(const auto& pair : m_property) {
        if(pair.first == "Name") {
            continue;
        }
        res.push_back(pair);
    }
    return res;
}

QVector<QPair<QString, QString> > Object::getProperty() const {
    return m_property;
}
