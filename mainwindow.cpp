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
    auto *promptLineEdit = new QLineEdit("C:/Dev/test/CSV_Files"); //temp
    auto *promptButton = new QPushButton("Open directory");
    auto *openFileDialogButton = new QPushButton("Choose a local directory...");
    dirPromptLayout->addWidget(promptLabel);
    dirPromptLayout->addWidget(promptLineEdit);
    dirPromptLayout->addWidget(promptButton);
    dirPromptLayout->addWidget(openFileDialogButton);
    dirPromptLayout->addStretch();

    // Stacked view
    auto *fileViewStacked = new QStackedWidget;
    fileViewStacked->addWidget(dirPromptWidget);
    fileViewStacked->addWidget(m_fileSystemView);

    // Open file from line edit
    connect(promptButton, &QPushButton::clicked,
            this, [this, fileViewStacked, promptLineEdit]()
            {
                QString dir = promptLineEdit->text();
                QFileInfo dirInfo(dir);
                if (dirInfo.isDir()){
                    setupTreeView(dir);
                    fileViewStacked->setCurrentIndex(1);
                    return;
                }
                // Directory not valid, inform user
            });
    // Open file dialog (explorer)
    connect(openFileDialogButton, &QPushButton::clicked,
            this, [this, fileViewStacked]
            {
                QFileDialog file_dialog(this);

                QString dir = file_dialog.getExistingDirectory(this, tr("Open file..."),
                                                               "C:/", QFileDialog::ShowDirsOnly);
                setupTreeView(dir);
                fileViewStacked->setCurrentIndex(1);
            });

    connect(m_fileSystemView, &QTreeView::clicked,
            this, [this](const QModelIndex &index)
            {

                QString filename = m_fileSystemModel->filePath(index);
                m_tableModel->loadCSV(filename);
            }
            );

    m_tableView = new QTableView(this);
    m_tableModel = new CSVTableModel(this);
    m_tableView->setModel(m_tableModel);



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
