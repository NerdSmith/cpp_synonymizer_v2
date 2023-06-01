#include "syndict.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

SynDict::SynDict()
{
}

bool SynDict::loadFromFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) || _files.contains(filename)) {
        qCritical() << "Cant load file";
        return false;
    }
    QTextStream in(&file);
    Table table;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList words = line.split(",");

        Row values;

        for (const QString& item : words) {
            values.append(item);
        }
        table.append(values);
    }

    _tables.append(table);
    _files.append(filename);

    qInfo() << "File " << filename << " is loaded";
    return true;
}

QString SynDict::findSyn(QString word)
{
    QString found;
    std::srand(std::time(nullptr));
    for (auto& table : _tables) {
        for (const Row& row : table) {
            if (row.contains(word)) {
                if (!found.isEmpty() && skipRepeat) {
                    continue;
                }
                int randomIndex = std::rand() % row.size();
                found = row[randomIndex];
                qInfo() << "Found syn for word: " << word << " -> " << found;
            }
        }
    }
    if (found.length() == 0) {
        return word;
    }
    return found;
}

QStringList SynDict::getFiles()
{
    return this->_files;
}

QString SynDict::synonimyze(QString text)
{
    QRegExp wordSeparator("\\b(\\w+)\\b");
    QStringList wordsAndPunctuation;
    int pos = 0;
    while ((pos = wordSeparator.indexIn(text, pos)) != -1) {
        wordsAndPunctuation << wordSeparator.cap(1);
        wordsAndPunctuation << text.mid(pos + wordSeparator.matchedLength(), 1);
        pos += wordSeparator.matchedLength();
    }

    for (int i = 0; i < wordsAndPunctuation.size(); i += 2) {
        QString word = wordsAndPunctuation[i];

        QString synonym = findSyn(word.toLower());

        wordsAndPunctuation[i] = synonym;
    }

    QString result;
    for (const QString& wordOrPunctuation : wordsAndPunctuation) {
        result += wordOrPunctuation;
    }

    return result;
}

void SynDict::rmByIdx(int idx)
{
    if (idx >= 0 && idx < this->_files.size()) {
        this->_files.removeAt(idx);
        this->_tables.removeAt(idx);
    }
}

SynDict::~SynDict()
{
}
