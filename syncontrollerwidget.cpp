#include "syncontrollerwidget.h"
#include "ui_syncontrollerwidget.h"
#include <QCloseEvent>
#include <QFileDialog>

SynControllerWidget::SynControllerWidget(bool& isClosed, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SynControllerWidget)
{
    this->isClosed = &isClosed;
    *(this->isClosed) = false;
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

void SynControllerWidget::on_deleteDictBtn_clicked()
{
    int selectedItemIdx = this->ui->openedDictsListWid->currentRow();

    if (selectedItemIdx != -1) {
        this->sd.rmByIdx(selectedItemIdx);
        delete this->ui->openedDictsListWid->takeItem(selectedItemIdx);
    }
}
