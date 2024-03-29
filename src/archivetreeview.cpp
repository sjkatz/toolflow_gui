/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   05/03/2012
****************************************************************************/

#include "archivetreeview.h"

#include <qdebug.h>
#include <QtGui/QMouseEvent>
#include <QtGui/QApplication>
#include <QtGui/QFileSystemModel>

#include <QUrl>
#include <QAbstractItemView>


ArchiveTreeView::ArchiveTreeView(QWidget *parent)
    : QTreeView(parent)
{
    //model = new QFileSystemModel;
    //model->setRootPath(QDir::currentPath());
    //setModel(model);
    //this->model();

    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDropIndicatorShown(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    //setAutoExpandDelay(100);

}

ArchiveTreeView::~ArchiveTreeView()
{
    //delete model;
}

void ArchiveTreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
         return;
    if (!pressedIndex.isValid())
         return;
     if ((event->pos() - pressedPos).manhattanLength()
          < QApplication::startDragDistance())
         return;
     startDrag( Qt::CopyAction/*model()->supportedDragActions()*/);

     /*//TEST
     QDrag *drag = new QDrag(this);
     QMimeData *mimeData = new QMimeData;

     mimeData->setText(pressedIndex.data().toString());
     drag->setMimeData(mimeData);

     Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);*/
 }

void ArchiveTreeView::mousePressEvent(QMouseEvent *event)
{
    pressedPos = event->pos();
    pressedIndex = indexAt(pressedPos);
    QTreeView::mousePressEvent(event);
}

void ArchiveTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    //qDebug()<<"Dragging Entered..";
    event->accept();
}

void ArchiveTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    pressedPos = event->pos();
    pressedIndex = indexAt(pressedPos);
//    QFileSystemModel* filemodel = qobject_cast<QFileSystemModel*>(model());
    expand(pressedIndex);

    event->accept();
}

void ArchiveTreeView::dropEvent(QDropEvent *event)
{
    pressedPos = event->pos();
    pressedIndex = indexAt(pressedPos);
    QFileSystemModel* filemodel = qobject_cast<QFileSystemModel*>(model());
    QList<QUrl> urlList;
    QModelIndex parent = pressedIndex.parent();
    QString destDir;
    urlList = event->mimeData()->urls(); // retrieve list of urls

    foreach(QUrl url, urlList) // iterate over list
    {
        int row = pressedIndex.row();
        filemodel->dropMimeData(event->mimeData(),Qt::CopyAction,row,0,parent);
        // if parent is null - add top level item (this parent)
        if (pressedIndex == QModelIndex()) destDir = filemodel->rootPath() + "/";
        else
        {
            QFileInfo fi = filemodel->fileInfo(pressedIndex);
            if(fi.isDir())
                if(isExpanded(pressedIndex))
                    destDir = fi.canonicalFilePath() + "/";
                else
                    destDir = fi.canonicalPath() + "/";
            else
                destDir = fi.canonicalPath() + "/";

        }

        QString source = url.toLocalFile();
        QString dest;
        QFileInfo info(source);
        if(info.isDir()){
            dest = destDir + info.baseName() +"/";
        }
        else{
            dest = destDir;
        }

        copyDir(source,dest);
    }
    event->acceptProposedAction();
}

bool ArchiveTreeView::copyDir(const QString &sourceName, const QString &destName)
{
    //qDebug()<<"Copy " << sourceName << " to " << destName;
    bool result = true;
    QFileInfo sourceInfo(sourceName);
    QDir sourceDir(sourceName);
    QDir destDir(destName);

    if(sourceInfo.isDir())
    {
        if (!destDir.exists(destName))
            destDir.mkdir(destDir.path());
        if (sourceDir.exists(sourceName)) {
            Q_FOREACH(QFileInfo info, sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
                if (info.isDir()) {
                    if(info.filePath().right(2) != ".."){
                     result = copyDir(info.absoluteFilePath(), destName + info.baseName()+ "/");
                    //qDebug() << "isDir : "<< info.absoluteFilePath() << destName + info.baseName() + "/";
                    }
                }
                else {
                    result = QFile::copy(info.absoluteFilePath(),destName + info.fileName());
                    //qDebug() << "isFile : "<< info.absoluteFilePath() << destName  + info.fileName();
                }

                if (!result) {
                    return result;
                }
            }

        }
    }
    else{
        result = QFile::copy(sourceInfo.absoluteFilePath(),destName + sourceInfo.fileName());
       // qDebug() << "isFile : "<< sourceInfo.absoluteFilePath() << destName  + sourceInfo.fileName();
    }


    return result;
}

bool ArchiveTreeView::removeDir(const QString &dirName, bool removeRoot)
{
    qDebug()<<"Remove " << dirName ;
    bool result = true;
    QFileInfo fi(dirName);
    QDir dir(dirName);

    if(fi.isDir()){
        if (dir.exists(dirName)) {
            Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
                if (info.isDir()) {
                    result = removeDir(info.absoluteFilePath());
                }
                else {
                    result = QFile::remove(info.absoluteFilePath());
                }

                if (!result) {
                    return result;
                }
            }
            if(removeRoot)
                result = dir.rmdir(dirName);
        }
    }
    else
    {
        result = QFile::remove(fi.absoluteFilePath());
    }

    return result;
}

