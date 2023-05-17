#ifndef QTREEWIDGETOBJECTITEM_H
#define QTREEWIDGETOBJECTITEM_H

#include <QTreeWidgetItem>

class Object;

class QTreeWidgetObjectItem : public QTreeWidgetItem
{
    Object* m_obj = nullptr;

public:
    QTreeWidgetObjectItem();
    void setPtr(Object* p);
    Object* getPtr();
};

#endif // QTREEWIDGETOBJECTITEM_H
