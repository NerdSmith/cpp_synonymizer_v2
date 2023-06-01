#include "mytextedit.h"
#include <QtWidgets>

MyTextEdit::MyTextEdit()
{
}

void MyTextEdit::setCurrentFile(const QString& fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(QFileInfo(fileName).fileName() + "[*]");
}

void MyTextEdit::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = QString("document%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
}

bool MyTextEdit::loadFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << QString("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString());
        QMessageBox::warning(this, tr("MDI"),
            tr("Cannot read file %1:\n%2.")
                .arg(fileName)
                .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QGuiApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    return true;
}

bool MyTextEdit::loadText(QString text)
{
    static int sequenceNumber = 1;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(text);
    QGuiApplication::restoreOverrideCursor();

    isUntitled = true;
    curFile = QString("document%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    return true;
}

bool MyTextEdit::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
        curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MyTextEdit::saveFile(const QString& fileName)
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << toPlainText();
        if (!file.commit()) {
            qCritical() << QString("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
            errorMessage = tr("Cannot write file %1:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        qCritical() << QString("Cannot open file %1 for writing:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        qWarning() << "MDI";
        QMessageBox::warning(this, tr("MDI"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    return true;
}
