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
#include <QFileDialog>
#include <QStandardPaths>

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
    auto *promptLabel = new QLabel("You haven't opened a folder yet.");
    auto *openFileDialogButton = new QPushButton("Open folder");
    dirPromptLayout->addWidget(promptLabel);
    dirPromptLayout->addWidget(openFileDialogButton);
    dirPromptLayout->addStretch();

    // Stacked view
    m_fileViewStacked = new QStackedWidget;
    m_fileViewStacked->addWidget(dirPromptWidget);
    m_fileViewStacked->addWidget(m_fileSystemView);

    // Open file dialog (explorer)
    connect(openFileDialogButton, &QPushButton::clicked,
            this, &MainWindow::openFolder);

    connect(m_fileSystemView, &QTreeView::clicked,
            this, &MainWindow::onTreeViewClicked);

    m_tableView = new QTableView(this);
    m_tableModel = new CSVTableModel(this);
    m_tableView->setModel(m_tableModel);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->addWidget(m_fileViewStacked);
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
}

void MainWindow::teardownTreeView()
{
    m_fileSystemView->setRootIndex(QModelIndex());
    m_fileSystemModel->setRootPath(QString());

    m_tableModel->clear();
}

void MainWindow::openFolder()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Open Folder"),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        QFileDialog::ShowDirsOnly
        );

    if (dir.isEmpty())
        return;

    setupTreeView(dir);
    m_fileViewStacked->setCurrentIndex(1);
}

void MainWindow::closeFolder()
{
    teardownTreeView();
    m_fileViewStacked->setCurrentIndex(0);
}

void MainWindow::openFile(const QString &filename)
{
    m_tableModel->loadCSV(filename);
}

void MainWindow::onTreeViewClicked(const QModelIndex &index)
{
    QString filename = m_fileSystemModel->filePath(index);
    openFile(filename);
}
