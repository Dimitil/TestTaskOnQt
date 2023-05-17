#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "listcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ListController m_lc;
    QString m_readedFrom;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void onItemClicked(QTreeWidgetItem* item, int col);
    void on_accept_clicked();

private:
    Ui::MainWindow *ui;
    void fillTree();
    void treePrepare();
    void addTreeRoot(const QString& name);
    void addObjectInTree(Object& obj);
    void addPropertyForItem(QTreeWidgetItem* item, Object& obj);
    void hideInputWidgets();
    void showInputWidgets(QTreeWidgetItem* item);

};
#endif // MAINWINDOW_H
