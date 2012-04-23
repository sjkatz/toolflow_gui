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
#include <QUndoStack>
//#include <QGraphicsScene>
#include <QProcess>

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
    QAction* action_export;
    QAction* action_save;
    QAction* action_delete;
    QAction* action_wire;
    QAction* action_select;
    QAction* action_undo;
    QAction* action_redo;
    QAction* action_execute;
    ArchiveTreeView* treeView;
    QUndoStack* undo_stack;
    QProcess* proc;
    QFileInfo project_file;

private slots:
    void actionOpen_Triggered();
    void actionExport_Triggered();
    void actionExecute_Triggered(QStringList args = QStringList());
    void rightMessage();
    void wrongMessage();
    void actionSave_Triggered();
    void actionWire_Triggered();
    void actionSelect_Triggered();
    void actionDelete_Triggered();
    void refreshTreeView();
    void treeView_doubleClicked(QModelIndex index);
    void treeProcessWidget_doubleClicked(QModelIndex index);
    void slider_valueChanged(int value);
    void itemMoved(QGraphicsItem*,QPointF);
    void addGraphicsItem(QGraphicsItem*);
    void undoStack_CleanChanged(bool is_clean);
    void copyFolder(QString sourceFolder, QString destFolder);


};

#endif // MAINWINDOW_H
