/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "archivetreeview.h"
#include "graphicsscene.h"
#include "graphicsview.h"

#include <QMainWindow>
//#include <QGraphicsScene>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupToolBar();
    void setupView();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    GraphicsScene* scene;
    GraphicsView* view;
    QAction* action_open;
    QAction* action_delete;
    QAction* action_wire;
    QAction* action_select;
    ArchiveTreeView* treeView;

private slots:
    void actionOpen_Triggered();
    void actionWire_Triggered();
    void actionSelect_Triggered();
    void actionDelete_Triggered();
    void refreshTreeView();
    void treeView_doubleClicked(QModelIndex index);
    void slider_valueChanged(int value);
    void scene_selectionChanged();
};

#endif // MAINWINDOW_H
