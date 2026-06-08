#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "csvtablemodel.h"

#include <QMainWindow>

class QSplitter;
class QTreeWidget;
class QTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QSplitter   *m_splitter  = nullptr;
    QTreeWidget *m_fileTree  = nullptr;
    QTableView  *m_tableView = nullptr;
    CSVTableModel *m_tableModel = nullptr;
};
#endif // MAINWINDOW_H
