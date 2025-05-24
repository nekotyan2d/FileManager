#include "ChooseDirWindow.h"

ChooseDirWindow::ChooseDirWindow(QWidget *parent) : FileManager(parent)
{
    ui->mainSplitter->widget(2)->deleteLater();
    delete previewWidget;
    previewWidget = nullptr;
    ui->mainSplitter->setSizes({300, 7000});
    
    delete ui->copyButton;
    ui->copyButton = nullptr;
    delete ui->moveButton;
    ui->moveButton = nullptr;
    delete ui->deleteButton;
    ui->deleteButton = nullptr;

    delete ui->horizontalLayout;
    ui->horizontalLayout = nullptr;


    window()->setWindowFlags(Qt::Dialog);

    actionsWidget = new QWidget();
    actionsLayout = new QHBoxLayout(actionsWidget);
    folderNameEdit = new QLineEdit(this);
    openBtn = new QPushButton(this);
    openBtn->setText("Открыть");

    actionsLayout->addWidget(folderNameEdit);
    actionsLayout->addWidget(openBtn);

    ui->contentLayout->addWidget(actionsWidget);

    connect(ui->listView, &QListView::clicked, this, [=](const QModelIndex &index){
        QString fullPath = index.data(Qt::UserRole + 2).toString();
        QFileInfo fileInfo(fullPath);
        if (fileInfo.isDir())
        {
            folderNameEdit->setText(fileInfo.fileName());
        }
    });

    connect(openBtn, &QPushButton::clicked, this, [this]() {
        selectedPath = folderNameEdit->text();
        accept();
    });
}

ChooseDirWindow::~ChooseDirWindow()
{
    delete ui;
}

int ChooseDirWindow::exec()
{
    setWindowModality(Qt::ApplicationModal);
    show();

    QEventLoop loop;
    // Connect accept/reject to loop.quit
    connect(this, &ChooseDirWindow::accepted, &loop, &QEventLoop::quit);
    connect(this, &ChooseDirWindow::rejected, &loop, &QEventLoop::quit);

    loop.exec();

    return dialogResult; // 1 for Accepted, 0 for Rejected
}

void ChooseDirWindow::closeEvent(QCloseEvent *event)
{
    emit rejected();
    QMainWindow::closeEvent(event);
}

void ChooseDirWindow::accept()
{
    dialogResult = 1;
    emit accepted();
    close();
}

void ChooseDirWindow::reject()
{
    dialogResult = 0;
    emit rejected();
    close();
}

QString ChooseDirWindow::chooseDirPath(QWidget *parent, QString dir)
{
    ChooseDirWindow window(parent);
    window.fileModel->setPath(dir.isEmpty() ? QDir::homePath() : dir);
    if (window.exec() == 1)
    {
        return window.selectedPath;
    }
    return QString();
}

void ChooseDirWindow::pathChanged(const QString &newPath){
    fileModel->setPath(newPath);
    ui->pathLineEdit->setText(newPath);
    folderNameEdit->setText(newPath);
}