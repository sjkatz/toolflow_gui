/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   05/03/2012
****************************************************************************/

#ifndef ARCHIVETREEVIEW_H
#define ARCHIVETREEVIEW_H

#include <QTreeView>
#include <QFileSystemModel>
#include <QTreeWidgetItem>

class ArchiveTreeView :public QTreeView {

public:
    ArchiveTreeView( QWidget *parent = 0);
    ~ArchiveTreeView();

    //QFileSystemModel* model;
    QPoint pressedPos;
    QModelIndex pressedIndex;
    bool copyDir(const QString &sourceName, const QString &destName);
    bool removeDir(const QString &dirName, bool removeRoot = false);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};



#endif // ARCHIVETREEVIEW_H
