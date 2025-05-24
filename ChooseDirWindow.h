#include "FileManager.h"
#include "QDialog"
#include <QEventLoop>

class ChooseDirWindow : public FileManager
{
    Q_OBJECT
public:
    explicit ChooseDirWindow(QWidget *parent = nullptr);
    ~ChooseDirWindow();

    static QString chooseDirPath(QWidget *parent, QString dir);
    int exec();
signals:
    void accepted();
    void rejected();
protected:
    void closeEvent(QCloseEvent *event) override;
    void pathChanged(const QString &newPath);
private:
    int dialogResult;

    void accept();
    void reject();

    QLineEdit *folderNameEdit;
    QString selectedPath;
    QPushButton *openBtn;
    QWidget *actionsWidget;
    QHBoxLayout *actionsLayout;
};