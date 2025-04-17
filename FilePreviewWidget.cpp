#include "FilePreviewWidget.h"
#include "ui_FilePreviewWidget.h"

FilePreviewWidget::FilePreviewWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::FilePreviewWidget)
{
    ui->setupUi(this);
    ui->imageLabel->hide();
    ui->textEdit->hide();
    ui->verticalLayout->setAlignment(Qt::AlignVCenter);

    ui->infoLabel->setText("Выберите файл для предпросмотра");
}

FilePreviewWidget::~FilePreviewWidget() {
    delete ui;
}

void FilePreviewWidget::clear() {
    ui->imageLabel->clear();
    ui->textEdit->clear();
    ui->infoLabel->setText("Выберите файл для предпросмотра");

    ui->imageLabel->hide();
    ui->textEdit->hide();
    ui->infoLabel->show();
}

void FilePreviewWidget::previewFile(const QString& path, bool isDir)
{
    ui->imageLabel->hide();
    ui->textEdit->hide();
    ui->infoLabel->hide();

    QFileInfo info(path);

    if (isDir) {
        QDir dir(path);
        int count = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).count();
        QIcon icon = QFileIconProvider().icon(info);
        ui->imageLabel->setPixmap(icon.pixmap(64, 64));
        ui->imageLabel->show();
        ui->infoLabel->setText(QString("%1 элементов").arg(count));
        ui->infoLabel->show();

        return;
    }

    QString suffix = info.suffix().toLower();

    QStringList imageFormats = {"png", "jpg", "jpeg", "bmp", "gif"};
    if (imageFormats.contains(suffix)) {
        QPixmap pix(path);
        int labelWidth = ui->imageLabel->width();
        int pixmapHeight = pix.height() * labelWidth / pix.width();
        QPixmap scaledPix = pix.scaled(labelWidth, pixmapHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(scaledPix);
        ui->imageLabel->show();

        return;
    }

    QStringList textFormats = {"txt", "cpp", "h", "py", "md", "log", "ini", "json", "xml", "html", "csv", "bat", "sh"};
    if (textFormats.contains(suffix)) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.read(2048);
            if(suffix == "md"){
                ui->textEdit->setMarkdown(content);
            } else if(suffix == "html"){
                ui->textEdit->setHtml(content);
            } else {
                ui->textEdit->setPlainText(content);
            }
            ui->textEdit->show();
        }
        return;
    }

    ui->infoLabel->setText("Невозможно выполнить предпросмотр файла");
    ui->infoLabel->show();
}