#include "syncontrollerwidget.h"
#include "ui_syncontrollerwidget.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QDebug>
#include "mainwindow.h"

SynControllerWidget::SynControllerWidget(bool& isClosed, MyTextEdit* targetTextedit, MainWindow* mw, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SynControllerWidget),
    _mw(mw)
{
    this->isClosed = &isClosed;
    *(this->isClosed) = false;
    this->target = targetTextedit;

    ui->setupUi(this);
}

void SynControllerWidget::closeEvent(QCloseEvent* event)
{
    *(this->isClosed) = true;
    event->accept();
}

SynControllerWidget::~SynControllerWidget()
{
    delete ui;
}

void SynControllerWidget::on_loadDictBtn_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", "", "SYN файлы (*.syn)");
    if (!filePath.isEmpty()) {
        this->sd.loadFromFile(filePath);
        this->ui->openedDictsListWid->clear();
        this->ui->openedDictsListWid->addItems(this->sd.getFiles());
    }
}

void SynControllerWidget::load4SD(QString filepath) {
//    this->sd.loadFromFile(filepath);
    this->ui->openedDictsListWid->clear();
    this->ui->openedDictsListWid->addItems(this->sd.getFiles());
}

void SynControllerWidget::load4SD(QStringList filepaths) {
    //    this->sd.loadFromFile(filepath);
    this->ui->openedDictsListWid->clear();
    this->ui->openedDictsListWid->addItems(filepaths);
}

void SynControllerWidget::on_deleteDictBtn_clicked()
{
    int selectedItemIdx = this->ui->openedDictsListWid->currentRow();

    if (selectedItemIdx != -1) {
        this->sd.rmByIdx(selectedItemIdx);
        delete this->ui->openedDictsListWid->takeItem(selectedItemIdx);
    }
}

void SynControllerWidget::on_performSyn_clicked()
{
    if (this->ui->openedDictsListWid->currentItem()) {
        QString filepath = this->ui->openedDictsListWid->currentItem()->text();
        this->sd.loadFromFile(filepath);
    }

    if (target && !target->isUntitle()) {
        QString data = target->toPlainText();
        QString res = sd.synonimyze(data);

        MyTextEdit* child = _mw->createNewTextEdit();
        const bool succeeded = child->loadText(res);
        if (succeeded) {
            child->show();
            qInfo() << "done";
        }
        else
            child->close();
        this->close();

    }
}

