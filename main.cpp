#include "FileManager.h"

#include <QApplication>
#include <QIcon>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileManager w;

    QFile styleFile(":/FileManager/Styles/app.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    }

    w.setWindowIcon(QIcon(":/FileManager/Resources/icon.png"));
    w.setMinimumSize(1000, 600);
    w.show();
    return a.exec();
}
