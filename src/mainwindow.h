#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "csvtablemodel.h"

#include <QMainWindow>

class QSplitter;
class QTreeView;
class QTableView;
class QFileSystemModel;
class QStackedWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    void setupTreeView(QString path);
    void teardownTreeView();

private slots:
    void openFolder();
    void closeFolder();
    void openFile(const QString &filename); //open file without opening a folder
    void onTreeViewClicked(const QModelIndex &index); //wraps openFile

private:
    QSplitter   *m_splitter  = nullptr;
    QTreeView *m_fileSystemView  = nullptr;
    QFileSystemModel *m_fileSystemModel = nullptr;
    QStackedWidget *m_fileViewStacked = nullptr;
    QTableView  *m_tableView = nullptr;
    CSVTableModel *m_tableModel = nullptr;
};
#endif // MAINWINDOW_H
