#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QToolBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
}

bool MainWindow::openControllerWidget()
{
    if (!isSynClosed && synController) {
        synController->parentWidget()->close();
    }

    if (QMdiSubWindow* activeSubWindow = ui->mdiArea->activeSubWindow())
        lastOpenedTextEdit = qobject_cast<MyTextEdit*>(activeSubWindow->widget());

    synController = new SynControllerWidget(isSynClosed, lastOpenedTextEdit, this);
//    ui->mdiArea->addSubWindow(synController);

//    synController->loadSyns();
    const QList<QMdiSubWindow*> subWindows = ui->mdiArea->subWindowList();
    QStringList l;
    for (QMdiSubWindow* window : subWindows) {
        MyTextEdit* mdiChild = qobject_cast<MyTextEdit*>(window->widget());
        if (mdiChild) {
            if (mdiChild->currentFile().endsWith(".syn")) {
//                synController->load4SD(mdiChild->currentFile());
                l.append(mdiChild->currentFile());
            }
        }
    }
    synController->load4SD(l);
    synController->show();
    statusBar()->showMessage("Last opened: " + lastOpenedTextEdit->currentFile());


    return true;
}

MyTextEdit* MainWindow::activeMdiChild()
{
    if (QMdiSubWindow* activeSubWindow = ui->mdiArea->activeSubWindow())
        return qobject_cast<MyTextEdit*>(activeSubWindow->widget());
    return nullptr;
}

void MainWindow::newFile()
{
    MyTextEdit* child = createNewTextEdit();
    child->newFile();
    child->show();
}

QMdiSubWindow* MainWindow::findMdiChild(const QString& fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    const QList<QMdiSubWindow*> subWindows = ui->mdiArea->subWindowList();
    for (QMdiSubWindow* window : subWindows) {
        MyTextEdit* mdiChild = qobject_cast<MyTextEdit*>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return nullptr;
}

bool MainWindow::openFile(const QString& fileName)
{
    //    if (QMdiSubWindow* existing = findMdiChild(fileName)) {
    //        ui->mdiArea->setActiveSubWindow(existing);
    //        return true;
    //    }
    const bool succeeded = loadFile(fileName);
    if (succeeded)
        qInfo() << "File loaded";
    return succeeded;
}

bool MainWindow::loadFile(const QString& fileName)
{
    MyTextEdit* child = createNewTextEdit();
    const bool succeeded = child->loadFile(fileName);
    if (succeeded)
        child->show();
    else
        child->close();
    return succeeded;
}

void MainWindow::open()
{
    const QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "SYN или TXT файлы (*.syn *.txt)");
    if (!fileName.isEmpty())
        openFile(fileName);
}

void MainWindow::saveAs()
{
    MyTextEdit* child = activeMdiChild();
    if (child && child->saveAs()) {
        qInfo() << "File saved";
    }
}

void MainWindow::synChild()
{
    if (QMdiSubWindow* activeSubWindow = ui->mdiArea->activeSubWindow()) {
        MyTextEdit* o = qobject_cast<MyTextEdit*>(activeSubWindow->widget());
        if (o) {
            qDebug() << "Target child found";

            if (!this->isSynClosed) {
                qInfo() << "Control is up";

                SynDict sd = synController->getSynDict();
                QString res = sd.synonimyze(o->toPlainText());

                MyTextEdit* child = createNewTextEdit();
                const bool succeeded = child->loadText(res);
                if (succeeded)
                    child->show();
                else
                    child->close();
            } else {
                qCritical() << "Control is down";
            }
        }
    }
}

MyTextEdit* MainWindow::createNewTextEdit()
{
    MyTextEdit* child = new MyTextEdit;
    ui->mdiArea->addSubWindow(child);

    return child;
}

void MainWindow::createActions()
{
    QMenu* fileMenu = menuBar()->addMenu("&File");
    QToolBar* fileToolBar = addToolBar("File");

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    newAct = new QAction(newIcon, "&New", this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip("Create a new file");
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction* openAct = new QAction(openIcon, "&Open...", this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip("Open an existing file");
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    saveAsAct = new QAction(saveAsIcon, "Save &As...", this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip("Save the document under a new name");
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);
    fileToolBar->addAction(saveAsAct);

    const QIcon openControllerWidIcon = QIcon::fromTheme("ctrl-wnd", QIcon(":/images/utility.png"));
    QAction* controlOpen = new QAction(openControllerWidIcon, "Contro&l", this);
    connect(controlOpen, &QAction::triggered, this, &MainWindow::openControllerWidget);
    fileMenu->addAction(controlOpen);
    fileToolBar->addAction(controlOpen);

//    const QIcon acceptWidIcon = QIcon::fromTheme("document-accept", QIcon(":/images/accept.png"));
//    QAction* acceptSyn = new QAction(acceptWidIcon, "Acce&pt", this);
//    connect(acceptSyn, &QAction::triggered, this, &MainWindow::synChild);
//    fileMenu->addAction(acceptSyn);
//    fileToolBar->addAction(acceptSyn);

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction* exitAct = fileMenu->addAction(exitIcon, "E&xit", qApp, &QApplication::closeAllWindows);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip("Exit the application");
    fileMenu->addAction(exitAct);
}

MainWindow::~MainWindow()
{
    delete ui;
}
