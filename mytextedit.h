#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <QTextEdit>

class MyTextEdit : public QTextEdit {
    Q_OBJECT
public:
    MyTextEdit();

    void newFile();
    bool loadFile(const QString& fileName);
    bool saveAs();
    bool saveFile(const QString& fileName);
    bool loadText(QString text);
    QString currentFile() { return curFile; }

private:
    void setCurrentFile(const QString& fileName);

    QString curFile;
    bool isUntitled;
};

#endif // TEXTVIEW_H
