#include "ChooseDirWindow.h"

ChooseDirWindow::ChooseDirWindow(QWidget *parent) : FileManager(parent)
{
    ui->mainSplitter->widget(2)->deleteLater();
    ui->mainSplitter->setSizes({300, 7000});

    QWidget *actionsWidget = new QWidget();
    QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
    QLineEdit *folderNameEdit = new QLineEdit(this);
    QPushButton *openBtn = new QPushButton(this);
    openBtn->setText("Открыть");

    actionsLayout->addWidget(folderNameEdit);
    actionsLayout->addWidget(openBtn);

    ui->contentLayout->addWidget(actionsWidget);
}