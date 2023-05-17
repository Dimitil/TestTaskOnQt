#include "qtreewidgetobjectitem.h"


QTreeWidgetObjectItem::QTreeWidgetObjectItem() {}

void QTreeWidgetObjectItem::setPtr(Object *p) {
    m_obj = p;
}

Object *QTreeWidgetObjectItem::getPtr() {
    return m_obj;
}
