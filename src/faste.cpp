#include "faste.h"
#include "ui_faste.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDesktopServices>

FastE::FastE(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FastE)
    , recentPath(QDir::homePath())
    , cppSourceTr("C++ source (*.cpp *.hpp *.c *.h);;All files (*)")
    , ctrlN(new QShortcut(QKeySequence("Ctrl+N"), this))
    , ctrlO(new QShortcut(QKeySequence("Ctrl+O"), this))
    , ctrlS(new QShortcut(QKeySequence("Ctrl+S"), this))
    , ctrlShiftS(new QShortcut(QKeySequence("Ctrl+Shift+S"), this))
    , ctrlW(new QShortcut(QKeySequence("Ctrl+W"), this))
    , plusButton(new QToolButton())
    , plusTabBar(new QTabBar())
{
    ui->setupUi(this);
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setMovable(true);
    this->centralWidget()->layout()->setContentsMargins(0,0,0,0);
    ConnectSignalToSlot();

    // Add another tab
    NewTab();
    AddPlusButton();
}

void FastE::AddPlusButton()
{
    connect(plusButton, SIGNAL(clicked()), this, SLOT(NewTab()));
    plusButton->setText(" + ");
    // This does not work with qt4
//    plusButton->setFixedHeight(ui->tabWidget->height());

    ui->tabWidget->addTab(plusTabBar, QString());
    ui->tabWidget->setTabEnabled(ui->tabWidget->count() - 1, false);

    ui->tabWidget->tabBar()->setTabButton(1, QTabBar::RightSide, plusButton);
    ui->tabWidget->setStyleSheet("QTabBar::tab:disabled { background-color: transparent; margin-left: 0px; margin-top: 0px;}");
}

void FastE::ConnectSignalToSlot()
{
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(NewTab()));
    connect(ctrlN, SIGNAL(activated()), this, SLOT(NewTab()));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(Open()));
    connect(ctrlO, SIGNAL(activated()), this, SLOT(Open()));

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(Save()));
    connect(ctrlS, SIGNAL(activated()), this, SLOT(Save()));

    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(SaveAs()));
    connect(ctrlShiftS, SIGNAL(activated()), this, SLOT(SaveAs()));

    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(CloseCurrent()));
    connect(ctrlW, SIGNAL(activated()), this, SLOT(CloseCurrent()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),this, SLOT(CloseTab(int)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),this, SLOT(tabChanged(int)));
}

QString FastE::GetTabName()
{
    QString tabName = "untitled.cpp";
    for (int i = 1; i < INT_MAX; i++) {
        int j = 0;
        for (j = 0; j < ui->tabWidget->count(); j++) {
            if (tabName == ui->tabWidget->tabText(j)) break;
        }
        if (j == ui->tabWidget->count()) break;
        tabName = "untitled_" + QString::number(i) + ".cpp";
    }
    return tabName;
}

bool FastE::Save()
{
    bool lStatus = false;
    PlainTextEdit* lCurrentPlainTextEdit = (PlainTextEdit*)ui->tabWidget->currentWidget();
    if (lCurrentPlainTextEdit->GetPath().isNull()) {
      lStatus = SaveAs();
    } else {
        QString saveFileName = lCurrentPlainTextEdit->GetPath() + "/" + ui->tabWidget->tabText(ui->tabWidget->currentIndex());
        QFile file(saveFileName);
        if (file.open((QIODevice::WriteOnly))) {
            QTextStream out(&file);
            out << lCurrentPlainTextEdit->toPlainText();
            file.close();

            lCurrentPlainTextEdit->SetChangedStatus(false);
            lStatus = true;
        } else {
            // TODO Handle cases when file cannot be opened.
        }
    }
    return lStatus;
}

bool FastE::SaveAs()
{
    bool lStatus = false;
    PlainTextEdit* lCurrentPlainTextEdit = (PlainTextEdit*)ui->tabWidget->currentWidget();
    QString lPath = lCurrentPlainTextEdit->GetPath().isNull() ? recentPath : lCurrentPlainTextEdit->GetPath();
    QString saveFileName = lPath + "/" + ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    saveFileName = QFileDialog::getSaveFileName(this, tr("Save As"), saveFileName, tr(cppSourceTr));

    if (!saveFileName.isNull()) {
        recentPath = QFileInfo(saveFileName).absolutePath();
        QFile file(saveFileName);
        if (file.open((QIODevice::WriteOnly))) {
            QTextStream out(&file);
            out << lCurrentPlainTextEdit->toPlainText();
            file.close();

            lCurrentPlainTextEdit->SetPath(recentPath);
            lCurrentPlainTextEdit->SetChangedStatus(false);
            lStatus = true;
        } else {
            // TODO Handle cases when file cannot be opened.
        }
    }
    return lStatus;
}

void FastE::NewTab()
{
    PlainTextEdit* lNewTab = new PlainTextEdit();
    connect(lNewTab, SIGNAL(modificationChanged(bool)), this, SLOT(textChanged()));
    ui->tabWidget->insertTab(ui->tabWidget->count() - 1, lNewTab, GetTabName());
    ui->tabWidget->setCurrentWidget(lNewTab);
}

void FastE::Open()
{
    PlainTextEdit* lCurrentPlainTextEdit = (PlainTextEdit*)ui->tabWidget->currentWidget();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), recentPath, tr(cppSourceTr));
    if (!fileName.isNull()) {
        recentPath = QFileInfo(fileName).absolutePath();
        lCurrentPlainTextEdit->SetPath(recentPath);
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray lFileContent = file.readAll();
            file.close();

            lCurrentPlainTextEdit->setPlainText(lFileContent);
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), QFileInfo(fileName).fileName());
            lCurrentPlainTextEdit->SetChangedStatus(false);
        } else {
            // TODO Handle cases when file cannot be opened.
        }
    }
}

void FastE::CloseCurrent()
{
    CloseTab(ui->tabWidget->currentIndex());
}

void FastE::CloseTab(int pIndex)
{
    bool lClose = false;
    PlainTextEdit* lEdit = (PlainTextEdit*)ui->tabWidget->widget(pIndex);
    if (lEdit->IsChanged()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Test", "Do you want to save changes?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            lClose = Save();
        }  else {
            lClose = (reply == QMessageBox::No);
        }
    } else {
        lClose = true;
    }
    if (lClose) {
        if (ui->tabWidget->count() != 1) {
            ui->tabWidget->removeTab(pIndex);
                delete lEdit;
        }
    }
}

void FastE::tabChanged(int tabNumber)
{
    if (plusTabBar == (QTabBar*)ui->tabWidget->widget(tabNumber)) {
        ui->tabWidget->setCurrentIndex(tabNumber - 1);
    }
}

void FastE::textChanged()
{
    ((PlainTextEdit*)sender())->SetChangedStatus(true);
}

FastE::~FastE()
{
    delete ctrlN;
    delete ctrlO;
    delete ctrlS;
    delete ctrlShiftS;
    delete ctrlW;
    delete plusButton;
    delete ui;
}
