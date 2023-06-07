#ifndef SYNCONTROLLERWIDGET_H
#define SYNCONTROLLERWIDGET_H

#include "syndict.h"
#include <QWidget>
#include "mytextedit.h"

class MainWindow;

namespace Ui {
class SynControllerWidget;
}

class SynControllerWidget : public QWidget {
    Q_OBJECT

public:
    explicit SynControllerWidget(
        bool& isClosed,
        MyTextEdit* targetTextedit,
        MainWindow* mw,
        QWidget* parent = nullptr
        );
    ~SynControllerWidget();

    void load4SD(QString filepath);
    void load4SD(QStringList filepaths);

    SynDict& getSynDict() { return sd; };

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_loadDictBtn_clicked();

    void on_deleteDictBtn_clicked();

    void on_performSyn_clicked();

private:
    SynDict sd;
    Ui::SynControllerWidget* ui;
    bool* isClosed;
    MyTextEdit* target;
    MainWindow* _mw;
};

#endif // SYNCONTROLLERWIDGET_H
