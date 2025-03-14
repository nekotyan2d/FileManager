#include "FileManager.h"
#include <QtWidgets/QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileManager w;

	w.setWindowIcon(QIcon(":/FileManager/Resources/icon.png"));
    w.show();
    return a.exec();
}
