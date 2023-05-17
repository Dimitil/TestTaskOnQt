#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "qtreewidgetobjectitem.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnWidth(0, 150);
    ui->treeWidget->setColumnWidth(1, 200);
    hideInputWidgets();
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onItemClicked(QTreeWidgetItem*,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered() {
    QString path = QFileDialog::getOpenFileName(this, "Open list of objects file");
    if(!m_lc.readObjectsList(path)) {
        QMessageBox::warning(this, "Error", "Cannot read file: " + path);
        return;
    }
    m_readedFrom = path;
    fillTree();

}

void MainWindow::on_actionSave_triggered() {
    if(m_readedFrom.isEmpty()) {
        QMessageBox::warning(this, "Error", "Cannot save.");
        return;
    }
    if(m_lc.saveObjectList(m_readedFrom)) {
        QMessageBox::information(this, "Save", "Succes.");
    } else {
        QMessageBox::warning(this, "Error", "Cannot save in file : " + m_readedFrom);
    }
}

void MainWindow::onItemClicked(QTreeWidgetItem* item, int col) {
    QString val = item->text(1);
    hideInputWidgets();
    if(val.isEmpty()) {
        return;
    } else {
        showInputWidgets(item);
    }
}

void MainWindow::on_accept_clicked() {
    QList<QTreeWidgetItem*> itemList = ui->treeWidget->selectedItems();
    if(itemList.isEmpty()) return;
    QTreeWidgetItem* item = itemList.first();
    if(ui->path->isVisible()) {
        item->setText(1, ui->path->text());
    }
    if(ui->isb->isVisible()) {
        item->setText(1, ui->isb->text());
    }
    if(ui->dsb->isVisible()) {
        item->setText(1, ui->dsb->text());
    }
    if(ui->cb->isVisible()) {
        item->setText(1, ui->cb->currentText());
    }
    QTreeWidgetObjectItem* objItem = dynamic_cast<QTreeWidgetObjectItem*>(item->parent());
    if(!objItem) {
        return;
    }
    Object* obj = objItem->getPtr();
    if(!obj) {
        return;
    }

    obj->addProperty(item->text(0), item->text(1));
}

void MainWindow::fillTree() {
    treePrepare();
    for(auto& obj : m_lc.getObjects()) {
        addObjectInTree(obj);
    }
}

void MainWindow::treePrepare() {
    ui->treeWidget->clear();
    QStringList names = m_lc.getObjectsName();
    for(const auto& name : names) {
        addTreeRoot(name);
    }
}

void MainWindow::addTreeRoot(const QString &name) {
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);
    treeItem->setText(0, name);
}

void MainWindow::addObjectInTree(Object &obj) {
    QTreeWidget *tr = ui->treeWidget;
    QString objName = obj.getName();
    QList<QTreeWidgetItem*> itemList = tr->findItems(objName, Qt::MatchExactly);
    if(itemList.size() != 1) {
        return;
    }
    QTreeWidgetItem* root = itemList.first();
    QTreeWidgetObjectItem* item = new QTreeWidgetObjectItem();
    item->setPtr(&obj);
    item->setText(0, objName);
    root->addChild(item);
    addPropertyForItem(item, obj);
}

void MainWindow::addPropertyForItem(QTreeWidgetItem *item, Object &obj) {
    if(!item) {
        return;
    }
    QVector<QPair<QString, QString>> properties = obj.getPropertyWithoutName();
    for(const auto& pair : properties) {
        QTreeWidgetItem* propItem = new QTreeWidgetItem();
        propItem->setText(0, pair.first);
        propItem->setText(1, pair.second);
        item->addChild(propItem);
    }
}

void MainWindow::hideInputWidgets() {
    ui->path->hide();
    ui->isb->hide();
    ui->dsb->hide();
    ui->cb->clear();
    ui->cb->hide();
    ui->label->hide();
    ui->accept->hide();
}

void MainWindow::showInputWidgets(QTreeWidgetItem *item) {
    QString name = item->parent()->text(0);
    QString val = item->text(1);
    QString property = item->text(0);
    QString description;
    ValType type = m_lc.getType(name, property, &description);
    QStringList list;
    int cbIndx;
    if(type == ValType::ERROR) {
        return;
    }
    switch (type) {
    case PATH:
        ui->path->setText(val);
        ui->path->show();
        break;
    case UINT:
        ui->isb->setValue(val.toUInt());
        ui->isb->show();
        break;
    case REAL:
        list = description.split(" ", Qt::SkipEmptyParts)[2].split(",", Qt::SkipEmptyParts);
        ui->dsb->setMinimum(list[0].toDouble());
        ui->dsb->setMaximum(list[1].toDouble());
        val = val.replace(",", ".");
        ui->dsb->setValue(val.toDouble());
        ui->label->setText("Minimum = " + list[0] + ", maximum = " + list[1]);
        ui->label->show();
        ui->dsb->show();
        break;
    case ENUM:
        list = description.split(" ", Qt::SkipEmptyParts)[2].split(",", Qt::SkipEmptyParts);
        ui->cb->addItems(list);
        cbIndx = ui->cb->findText(val);
        ui->cb->setCurrentIndex(cbIndx);
        ui->cb->show();
        break;
    case ERROR:
    default:
        break;
    }
    ui->accept->show();
}
