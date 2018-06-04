#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QPlainTextEdit>

class PlainTextEdit : public QPlainTextEdit
{
public:
    PlainTextEdit()
        : m_path(QString::null)
        , m_changed(false)
    { }
public:
    void SetPath(QString pStr) { m_path = pStr; }
    QString GetPath() { return m_path; }
    bool IsChanged() { return m_changed; }
    void SetChangedStatus(bool pChanged) { m_changed = pChanged; }

private:
    QString m_path;
    bool m_changed;
};

#endif // PLAINTEXTEDIT_H
