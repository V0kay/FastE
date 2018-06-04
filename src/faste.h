#ifndef FASTE_H
#define FASTE_H

#include <QMainWindow>
#include <QDir>
#include <QShortcut>
#include <QToolButton>

#include "plaintextedit.h"

namespace Ui {
class faste;
}

class FastE : public QMainWindow
{
    Q_OBJECT

public:
    explicit FastE (QWidget *parent = 0);
    ~FastE();

private:
    Ui::FastE *ui;
    QString recentPath;
    const char* cppSourceTr;
    QShortcut* ctrlN;
    QShortcut* ctrlO;
    QShortcut* ctrlS;
    QShortcut* ctrlShiftS;
    QShortcut* ctrlW;
    QToolButton* plusButton;
    QTabBar* plusTabBar;

private:
    QString GetTabName();
    void ConnectSignalToSlot();
    void AddPlusButton();

public slots:
    void NewTab();
    void Open();
    bool Save();
    bool SaveAs();
    void CloseCurrent();
    void CloseTab(int pIndex);
    void textChanged();
    void tabChanged(int);
};

#endif // FASTE_H
