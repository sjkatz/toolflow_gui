#include "propertydialog.h"
#include "ui_propertydialog.h"

#include "propertymodel.h"

PropertyDialog::PropertyDialog(Component* component, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertyDialog)
{
    ui->setupUi(this);

    setComponent(component);
    refresh();
}

PropertyDialog::~PropertyDialog()
{
    delete ui;
}

Component* PropertyDialog::component()
{
    return d_component;
}

void PropertyDialog::setComponent(Component* component)
{
    d_component = component;
}

QTableView* PropertyDialog::view()
{
    return ui->tableView;
}

void PropertyDialog::refresh()
{
//    QString content;
//    QMapIterator<QString, QVariant> i(d_component->data());
//    while (i.hasNext()) {
//        i.next();
//       // content << i.key() << ": " << i.value() << "\n";
//        content = content + i.key() + " : " + i.value().toString() + "\n";
//    }
//    ui->plainTextEdit->setPlainText(content);

    PropertyModel* propertyModel = new PropertyModel();
    propertyModel->setPropertyMap(d_component->data());

    ui->tableView->setModel(propertyModel);
    ui->tableView->setAlternatingRowColors(true);
}
