#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "csvtablemodel.h"

#include <QMainWindow>

class QSplitter;
class QTreeView;
class QTableView;
class QFileSystemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void setupTreeView(QString path);

private:
    QSplitter   *m_splitter  = nullptr;
    QTreeView *m_fileSystemView  = nullptr;
    QFileSystemModel *m_fileSystemModel = nullptr;
    QTableView  *m_tableView = nullptr;
    CSVTableModel *m_tableModel = nullptr;
};
#endif // MAINWINDOW_H
