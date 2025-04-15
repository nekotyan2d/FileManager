/********************************************************************************
** Form generated from reading UI file 'FileManager.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEMANAGER_H
#define UI_FILEMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileManager
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QSplitter *mainSplitter;
    QWidget *sideWidget;
    QVBoxLayout *sideLayout;
    QTreeView *treeView;
    QWidget *contentWidget;
    QVBoxLayout *contentLayout;
    QHBoxLayout *topBarLayout;
    QToolButton *refreshButton;
    QLineEdit *pathLineEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *moveButton;
    QPushButton *copyButton;
    QPushButton *deleteButton;
    QSpacerItem *horizontalSpacer;
    QListView *listView;
    QWidget *previewWidget;
    QVBoxLayout *previewLayout;

    void setupUi(QMainWindow *FileManager)
    {
        if (FileManager->objectName().isEmpty())
            FileManager->setObjectName("FileManager");
        FileManager->resize(596, 400);
        centralWidget = new QWidget(FileManager);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        mainSplitter = new QSplitter(centralWidget);
        mainSplitter->setObjectName("mainSplitter");
        mainSplitter->setOrientation(Qt::Orientation::Horizontal);
        sideWidget = new QWidget(mainSplitter);
        sideWidget->setObjectName("sideWidget");
        sideLayout = new QVBoxLayout(sideWidget);
        sideLayout->setSpacing(6);
        sideLayout->setContentsMargins(11, 11, 11, 11);
        sideLayout->setObjectName("sideLayout");
        sideLayout->setContentsMargins(0, 0, 0, 0);
        treeView = new QTreeView(sideWidget);
        treeView->setObjectName("treeView");

        sideLayout->addWidget(treeView);

        mainSplitter->addWidget(sideWidget);
        contentWidget = new QWidget(mainSplitter);
        contentWidget->setObjectName("contentWidget");
        contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->setSpacing(6);
        contentLayout->setContentsMargins(11, 11, 11, 11);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(8, 8, 8, 8);
        topBarLayout = new QHBoxLayout();
        topBarLayout->setSpacing(6);
        topBarLayout->setObjectName("topBarLayout");
        refreshButton = new QToolButton(contentWidget);
        refreshButton->setObjectName("refreshButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/FileManager/Resources/refresh.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        refreshButton->setIcon(icon);

        topBarLayout->addWidget(refreshButton);

        pathLineEdit = new QLineEdit(contentWidget);
        pathLineEdit->setObjectName("pathLineEdit");
        QFont font;
        font.setPointSize(12);
        pathLineEdit->setFont(font);

        topBarLayout->addWidget(pathLineEdit);


        contentLayout->addLayout(topBarLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(8);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        moveButton = new QPushButton(contentWidget);
        moveButton->setObjectName("moveButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(moveButton->sizePolicy().hasHeightForWidth());
        moveButton->setSizePolicy(sizePolicy);
        moveButton->setFont(font);
        moveButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        moveButton->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/FileManager/Resources/move.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        moveButton->setIcon(icon1);
        moveButton->setIconSize(QSize(36, 24));

        horizontalLayout->addWidget(moveButton);

        copyButton = new QPushButton(contentWidget);
        copyButton->setObjectName("copyButton");
        sizePolicy.setHeightForWidth(copyButton->sizePolicy().hasHeightForWidth());
        copyButton->setSizePolicy(sizePolicy);
        copyButton->setFont(font);
        copyButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        copyButton->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/FileManager/Resources/copy.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        copyButton->setIcon(icon2);
        copyButton->setIconSize(QSize(36, 24));

        horizontalLayout->addWidget(copyButton);

        deleteButton = new QPushButton(contentWidget);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setEnabled(true);
        sizePolicy.setHeightForWidth(deleteButton->sizePolicy().hasHeightForWidth());
        deleteButton->setSizePolicy(sizePolicy);
        deleteButton->setMinimumSize(QSize(28, 0));
        deleteButton->setFont(font);
        deleteButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/FileManager/Resources/delete.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        deleteButton->setIcon(icon3);
        deleteButton->setIconSize(QSize(36, 24));

        horizontalLayout->addWidget(deleteButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalLayout->setStretch(0, 1);

        contentLayout->addLayout(horizontalLayout);

        listView = new QListView(contentWidget);
        listView->setObjectName("listView");
        listView->setFrameShape(QFrame::Shape::NoFrame);

        contentLayout->addWidget(listView);

        mainSplitter->addWidget(contentWidget);
        previewWidget = new QWidget(mainSplitter);
        previewWidget->setObjectName("previewWidget");
        previewLayout = new QVBoxLayout(previewWidget);
        previewLayout->setSpacing(6);
        previewLayout->setContentsMargins(11, 11, 11, 11);
        previewLayout->setObjectName("previewLayout");
        previewLayout->setContentsMargins(0, 0, 0, 0);
        mainSplitter->addWidget(previewWidget);

        verticalLayout->addWidget(mainSplitter);

        FileManager->setCentralWidget(centralWidget);

        retranslateUi(FileManager);

        QMetaObject::connectSlotsByName(FileManager);
    } // setupUi

    void retranslateUi(QMainWindow *FileManager)
    {
        FileManager->setWindowTitle(QCoreApplication::translate("FileManager", "FileManager", nullptr));
        refreshButton->setText(QCoreApplication::translate("FileManager", "...", nullptr));
#if QT_CONFIG(tooltip)
        moveButton->setToolTip(QCoreApplication::translate("FileManager", "\320\237\320\265\321\200\320\265\320\274\320\265\321\201\321\202\320\270\321\202\321\214", nullptr));
#endif // QT_CONFIG(tooltip)
        moveButton->setText(QString());
#if QT_CONFIG(tooltip)
        copyButton->setToolTip(QCoreApplication::translate("FileManager", "\320\232\320\276\320\277\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
#endif // QT_CONFIG(tooltip)
        copyButton->setText(QString());
#if QT_CONFIG(tooltip)
        deleteButton->setToolTip(QCoreApplication::translate("FileManager", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
#endif // QT_CONFIG(tooltip)
        deleteButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FileManager: public Ui_FileManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEMANAGER_H
