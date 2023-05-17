#include "listcontroller.h"
#include <QFile>
#include <QDir>

ListController::ListController()
    : m_propertiesPath(QDir::currentPath() + QDir::separator() + "metaObject.txt")
{
    if(!readProperty()) {
        throw QString("Cannot read property files from " + m_propertiesPath);
    }
}


bool ListController::readProperty() {
    QFile file(m_propertiesPath);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream in(&file);

    QString line = in.readLine();
    while(!in.atEnd()) {
        if(line == "#") {
            QString name = in.readLine();
            QStringList properties;
            line = in.readLine();
            while(line != "#") {
                properties << line;
                line = in.readLine();
                if(line.isEmpty()) {
                    break;
                }
            }
            m_properties[name] = properties;
        }
    }
    file.close();

    return true;
}

bool ListController::readObjectsList(const QString &path) {
    if(path.isEmpty()) {
        return false;
    }

    m_vec.clear();
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream in(&file);

    QString line = in.readLine();
    line = line.trimmed();
    while(!in.atEnd()) {
        if(line == "#") {
            Object obj;
            line = "next"; // refact this
            while(line != "#") {
                if(line.isEmpty()) {
                    break;
                }
                line = in.readLine();
                line = line.trimmed();
                obj.addProperty(line);
            }
            m_vec.push_back(obj);
        }
    }
    file.close();
    return true;
}

bool ListController::saveObjectList(const QString &path) {
    if(path.isEmpty()) {
        return false;
    }
    QFile outputFile(path);
    outputFile.open(QIODevice::WriteOnly);
    if(!outputFile.isOpen())
    {
        return false;
    }

    QTextStream os(&outputFile);

    for(const auto& obj : m_vec) {
        os << "#";
        Qt::endl(os);
        os << "Name " << obj.getName();
        Qt::endl(os);
        for(const auto& prop : obj.getPropertyWithoutName()) {
                os << prop.first << " " << prop.second;
                Qt::endl(os);
            }
    }
    Qt::endl(os);
    outputFile.close();
    return true;
}

QVector<Object> &ListController::getObjects() {
    return m_vec;
}

QStringList ListController::getObjectsName() const {
    return m_properties.keys();
}

ValType ListController::getType(const QString &name, const QString &property, QString *description) {
    if(!m_properties.contains(name)) {
        return ValType::ERROR;
    }
    QStringList propList = m_properties[name];
    for(const auto& str : propList) {
        if(str.contains(property)) {
            *description = str;
            QString valType = str.split(" ", Qt::SkipEmptyParts)[1];
            if(valType == "PATH") {
                return ValType::PATH;
            } else if (valType == "UINT") {
                return ValType::UINT;
            } else if (valType == "REAL") {
                return ValType::REAL;
            } else if (valType == "ENUM") {
                return ValType::ENUM;
            }
        }
    }
    return ValType::ERROR;
}
