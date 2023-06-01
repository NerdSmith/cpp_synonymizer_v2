#ifndef SYNCONTROLLERWIDGET_H
#define SYNCONTROLLERWIDGET_H

#include "syndict.h"
#include <QWidget>

namespace Ui {
class SynControllerWidget;
}

class SynControllerWidget : public QWidget {
    Q_OBJECT

public:
    explicit SynControllerWidget(bool& isClosed, QWidget* parent = nullptr);
    ~SynControllerWidget();

    SynDict& getSynDict() { return sd; };

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_loadDictBtn_clicked();

    void on_deleteDictBtn_clicked();

private:
    SynDict sd;
    Ui::SynControllerWidget* ui;
    bool* isClosed;
};

#endif // SYNCONTROLLERWIDGET_H
