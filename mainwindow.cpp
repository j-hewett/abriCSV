#include "mainwindow.h"

#include <QSplitter>
#include <QTableView>
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1200, 750);

    m_fileTree = new QTreeWidget(this);
    m_fileTree->setHeaderLabel("Files");

    m_tableView = new QTableView(this);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->addWidget(m_fileTree);
    m_splitter->addWidget(m_tableView);
    m_splitter->setSizes({200, 1000});

    setCentralWidget(m_splitter);
}
