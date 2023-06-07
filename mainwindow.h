#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mytextedit.h"
#include "syncontrollerwidget.h"
#include <QMainWindow>
#include <QMdiSubWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    MyTextEdit* createNewTextEdit();

private:
    void createActions();
    void newFile();
    void open();
    void saveAs();
    void synChild();

    bool openControllerWidget();
    MyTextEdit* activeMdiChild();
    QMdiSubWindow* findMdiChild(const QString& fileName);
    bool openFile(const QString& fileName);
    bool loadFile(const QString& fileName);

    Ui::MainWindow* ui;

    QAction* newAct;
    QAction* saveAsAct;
    SynControllerWidget* synController = nullptr;
    bool isSynClosed = true;
    MyTextEdit* lastOpenedTextEdit = nullptr;
};
#endif // MAINWINDOW_H
