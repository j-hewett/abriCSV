#include "mainwindow.h"

#include <QSplitter>
#include <QTableView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    int windowWidth = 1200;
    int windowHeight = 750;
    resize(windowWidth, windowHeight);
    int fileViewWidth = 300;
    int tableViewWidth = windowWidth - fileViewWidth;

    // File system view layout
    m_fileSystemView = new QTreeView;
    m_fileSystemModel = new QFileSystemModel(this);
    QString path = "C:/";

    // Directory prompt widget
    auto *dirPromptWidget = new QWidget;
    auto *dirPromptLayout = new QVBoxLayout(dirPromptWidget);
    auto *promptLabel = new QLabel("Enter directory path:");
    auto *promptLineEdit = new QLineEdit;
    auto *promptButton = new QPushButton("Open");
    dirPromptLayout->addWidget(promptLabel);
    dirPromptLayout->addWidget(promptLineEdit);
    dirPromptLayout->addWidget(promptButton);
    dirPromptLayout->addStretch();

    // Stacked view
    auto *fileViewStacked = new QStackedWidget;
    fileViewStacked->addWidget(dirPromptWidget);
    fileViewStacked->addWidget(m_fileSystemView);

    connect(promptButton, &QPushButton::clicked,
            this, [this, fileViewStacked, promptLineEdit]()
            {
                QString path = promptLineEdit->text();
                setupTreeView(path);
                fileViewStacked->setCurrentIndex(1);
    });

    m_tableView = new QTableView(this);
    m_tableModel = new CSVTableModel(this);
    m_tableView->setModel(m_tableModel);
    m_tableModel->loadCSV("test.csv");

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->addWidget(fileViewStacked);
    m_splitter->addWidget(m_tableView);
    m_splitter->setSizes({fileViewWidth, tableViewWidth});

    setCentralWidget(m_splitter);
}

void MainWindow::setupTreeView(QString path)
{
    m_fileSystemModel->setRootPath(path);

    m_fileSystemView->setModel(m_fileSystemModel);
    m_fileSystemView->setRootIndex(m_fileSystemModel->index(m_fileSystemModel->rootPath()));
    m_fileSystemView->setContextMenuPolicy(Qt::CustomContextMenu);

    // directory connections
}
