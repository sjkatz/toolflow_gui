#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include "component.h"
#include <QDialog>
#include <QTableView>

namespace Ui {
    class PropertyDialog;
}

class PropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertyDialog(Component* component, QWidget *parent = 0);
    ~PropertyDialog();

    Component* component();
    void setComponent(Component* component);

    QTableView* view();

private:
    Ui::PropertyDialog *ui;
    Component* d_component;

private slots:
    void refresh();
};

#endif // PROPERTYDIALOG_H
