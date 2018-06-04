#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class TabWidget : public QTabWidget
{
public:
    TabWidget(QWidget* parent=0) {
      
    }
    QTabBar* tabBar() const { return (QTabWidget::tabBar()); }
    virtual ~TabWidget() {}
};

#endif // PLAINTEXTEDIT_H
