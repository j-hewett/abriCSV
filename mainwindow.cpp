#include "mainwindow.h"

#include <QSplitter>
#include <QTableView>
#include <QTreeView>
#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    int windowWidth = 1200;
    int windowHeight = 750;
    resize(windowWidth, windowHeight);
    int fileViewWidth = 300;
    int tableViewWidth = windowWidth - fileViewWidth;

    m_fileSystemView = new QTreeView(this);
    m_fileSystemModel = new QFileSystemModel(this);

    QString path = "C:/";
    setupTreeView(path);

    m_tableView = new QTableView(this);
    m_tableModel = new CSVTableModel(this);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->addWidget(m_fileSystemView);
    m_splitter->addWidget(m_tableView);
    m_splitter->setSizes({fileViewWidth, tableViewWidth});

    setCentralWidget(m_splitter);
}

void MainWindow::setupTreeView(QString path)
{
    m_fileSystemModel = new QFileSystemModel;
    m_fileSystemModel->setRootPath(path);

    m_fileSystemView = new QTreeView;
    m_fileSystemView->setModel(m_fileSystemModel);
    m_fileSystemView->setRootIndex(m_fileSystemModel->index(m_fileSystemModel->rootPath()));
    m_fileSystemView->setContextMenuPolicy(Qt::CustomContextMenu);

    // directory connections
}
