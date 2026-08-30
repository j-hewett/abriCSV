#include "mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
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

    createMenuBar();

    // Left panel -----------------------------------

    auto *leftPanelWidget = new QWidget;
    auto *leftPanelLayout = new QVBoxLayout(leftPanelWidget);

    // File system view layout
    m_fileSystemView = new QTreeView;
    m_fileSystemModel = new QFileSystemModel(this);
    QString path = "C:/";

    // Directory prompt widget
    auto *dirPromptWidget = new QWidget;
    auto *dirPromptLayout = new QVBoxLayout(dirPromptWidget);
    auto *promptLabel = new QLabel("You haven't opened a folder yet.");
    auto *openFileDialogButton = new QPushButton("Open folder");
    dirPromptLayout->addStretch();
    dirPromptLayout->addWidget(promptLabel, 0, Qt::AlignHCenter);
    dirPromptLayout->addWidget(openFileDialogButton);
    dirPromptLayout->addStretch();

    // Stacked view
    m_fileViewStacked = new QStackedWidget;
    m_fileViewStacked->addWidget(dirPromptWidget);
    m_fileViewStacked->addWidget(m_fileSystemView);

    // Filename hint
    m_filenameHint = new QLabel("No file currently open.");

    auto *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);

    leftPanelLayout->addWidget(m_fileViewStacked);
    leftPanelLayout->addWidget(separator);
    leftPanelLayout->addWidget(m_filenameHint);

    // Connections etc -------------------------------

    // Open file dialog (explorer)
    connect(openFileDialogButton, &QPushButton::clicked,
            this, &MainWindow::openFolder);

    connect(m_fileSystemView, &QTreeView::clicked,
            this, &MainWindow::onTreeViewClicked);

    m_tableView = new QTableView(this);
    m_tableModel = new CSVTableModel(this);
    m_tableView->setModel(m_tableModel);

    m_splitter = new QSplitter(Qt::Horizontal, this);
    m_splitter->addWidget(leftPanelWidget);
    m_splitter->addWidget(m_tableView);
    m_splitter->setSizes({fileViewWidth, tableViewWidth});

    setCentralWidget(m_splitter);
}

void MainWindow::createMenuBar()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));

    m_openFolderAction = new QAction(tr("&Open folder..."), this);
    m_openFolderAction->setShortcut(QKeySequence::Open); //ctrl-o
    connect(m_openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);
    m_fileMenu->addAction(m_openFolderAction);

    m_closeFolderAction = new QAction(tr("&Close folder"), this);
    m_closeFolderAction->setEnabled(false); //nothing open
    connect(m_closeFolderAction, &QAction::triggered, this, &MainWindow::closeFolder);
    m_fileMenu->addAction(m_closeFolderAction);

    m_openFileAction = new QAction(tr("Open file..."), this);
    connect(m_openFileAction, &QAction::triggered, this, &MainWindow::promptOpenFile);
    m_fileMenu->addAction(m_openFileAction);
}

void MainWindow::setupTreeView(QString path)
{
    m_fileSystemModel->setRootPath(path);

    m_fileSystemView->setModel(m_fileSystemModel);
    m_fileSystemView->setRootIndex(m_fileSystemModel->index(m_fileSystemModel->rootPath()));
    m_fileSystemView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_fileSystemView->hideColumn(2);
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
    m_closeFolderAction->setEnabled(true);
}

void MainWindow::closeFolder()
{
    teardownTreeView();
    m_fileViewStacked->setCurrentIndex(0);
    m_closeFolderAction->setEnabled(false);
}

void MainWindow::openFile(const QString &filename)
{
    m_tableModel->loadCSV(filename);
    QFileInfo info(filename);
    m_filenameHint->setText(info.fileName());
}

void MainWindow::promptOpenFile()
{
    // Todo: some UI indication of which file is open
    // If a folder isn't open, or the file open doesn't belong to
    // the open folder, there should be some indication there too
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open CSV File"),
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        tr("CSV Files (*.csv)")
        );
    if (filename.isEmpty())
        return;
    openFile(filename);
}

void MainWindow::onTreeViewClicked(const QModelIndex &index)
{
    QString filename = m_fileSystemModel->filePath(index);
    openFile(filename);
}
