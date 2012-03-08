/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "component.h"
#include "constants.h"
#include "parser.h"

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QFileDialog>
#include <QFileInfo>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QSplitter>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalSlider->hide();

    setupToolBar();
    setupView();


    //layout = new QGridLayout(ui->treeView_Container);
   // ui->treeView_Container->setMaximumWidth(150);
    treeView = new ArchiveTreeView();
    //layout->addWidget(treeView);
    refreshTreeView();
    treeView->acceptDrops();


    connect(treeView,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(treeView_doubleClicked(QModelIndex)));
    connect(ui->verticalSlider,SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));


    QSplitter *splitter = new QSplitter(ui->mainWidget_Container);
    splitter->addWidget(treeView);
    splitter->addWidget(view);
    QGridLayout* layout = new QGridLayout(ui->mainWidget_Container);
    layout->addWidget(splitter);
    splitter->setStretchFactor(1,1);
}

MainWindow::~MainWindow()
{
    delete treeView;
    delete action_open;
    delete action_delete;
    delete action_wire;
    delete action_select;
    delete scene;
    delete ui;
}

void MainWindow::setupToolBar()
{
    ui->mainToolBar->addWidget(new QToolBar);
    action_open = ui->mainToolBar->addAction(QIcon(ICON_OPEN),"Open");
    connect(action_open,SIGNAL(triggered()), this, SLOT(actionOpen_Triggered()));

    ui->mainToolBar->addSeparator();

    action_wire = ui->mainToolBar->addAction(QIcon(ICON_WIRE),"Wire");
    action_wire->setCheckable(true);
    connect(action_wire,SIGNAL(triggered()), this, SLOT(actionWire_Triggered()));

    action_select = ui->mainToolBar->addAction(QIcon(ICON_SELECT),"Select");
    action_select->setCheckable(true);
    action_select->setChecked(true);
    connect(action_select,SIGNAL(triggered()), this, SLOT(actionSelect_Triggered()));
}

 void MainWindow::setupView()
 {
     view = new GraphicsView();

     scene = new GraphicsScene();
     view->setScene(scene);
     //view->scale(10,10);

    //scene->addLine(500,0,500,1000,Qt::DashLine);
     //Populate the scene
//     QPen pengrid;
//     pengrid.setWidth(2);
//     pengrid.setColor(QColor("#D8E5E5"));
//     pengrid.setStyle(Qt::DotLine);
//     QPen penpage;
//     penpage.setWidth(2);
//     //penpage.setColor(QColor("#D8E5E5"));
//     //penpage.setStyle(Qt::DotLine);
//     QPainter* painter = new QPainter(view);
//     painter->setPen(penpage);
     for(int x = 0; x <= 1000; x = x + 50) {
         for(int y = 0; y <= 1000; y = y + 50) {
             //scene->addRect(x, y, 24, 24,pen);
            //scene->addLine(x,y,x,1000,Qt::DashDotDotLine);
            //scene->addLine(x,y,1000,y,Qt::DashDotDotLine);
             if(x % 100 == 0 && y % 100 == 0) {
                 scene->addRect(x, y, 2, 2);
                 //scene->addLine(x,0,x,1000,penpage);
                 //scene->addLine(0,y,1000,y,penpage);
                // painter->drawLine(0,y,1000,y);

                 QString pointString;
                 QTextStream stream(&pointString);
                 stream << "(" << x << "," << y << ")";
                 //QGraphicsTextItem* item = scene->addText(pointString);
                 //item->setPos(x, y);
             } else {
                 scene->addRect(x, y, 1,1);
                 //scene->addLine(0,y+5,1000,y+5,pen);
                 //scene->addLine(x,0,x,1000,pengrid);
                 //scene->addLine(0,y,1000,y,pengrid);

             }
         }

     }
     view->setSceneRect(0, 0, 1000, 1000);
     view->SetCenter(QPointF(500, 500));
     //scene->setBackgroundBrush(QPixmap(QApplication::applicationDirPath() + "/../images/back.png"));
    // scene->setBackgroundBrush(QColor("#d3d3d3"));
    // view->centerOn(0,0);

     view->setContextMenuPolicy(Qt::ActionsContextMenu);
     action_delete = new QAction(QIcon(ICON_DELETE),"Delete",this);
     action_delete->setShortcut(QKeySequence(SHORTCUT_DELETE));
     view->addAction(action_delete);

     connect(action_delete,SIGNAL(triggered()), this, SLOT(actionDelete_Triggered()));
     connect(scene,SIGNAL(selectionChanged()), this, SLOT(scene_selectionChanged()));
 }

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::actionOpen_Triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open File"),
            QApplication::applicationDirPath()+"/../symbols",
            tr("All Files(*.*)") );

    if(fileName.isEmpty())
        return;

    Component* comp = Parser::toComponent(fileName);
    scene->addItem( comp);
    //comp->setPos(500,500);
}

void MainWindow::actionWire_Triggered()
{
    scene->setMode(GraphicsScene::InsertLine);
    foreach(QGraphicsItem* item, scene->selectedItems())
        item->setSelected(false);
   foreach(QAction* action, ui->mainToolBar->actions())
   {
        action->setChecked(false);
   }
   action_wire->setChecked(true);
   QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::actionSelect_Triggered()
{
    scene->setMode(GraphicsScene::MoveItem);
    foreach(QAction* action, ui->mainToolBar->actions())
    {
         action->setChecked(false);
    }
    action_select->setChecked(true);
    QApplication::restoreOverrideCursor();
}

void MainWindow::actionDelete_Triggered()
{
    foreach(QGraphicsItem* item , scene->selectedItems())
    {
        scene->removeItem(item);
    }
}

void MainWindow::refreshTreeView()
{
    QString archiveDirectory =  QApplication::applicationDirPath()+"/../symbols/";

    QFileSystemModel* filemodel =  new QFileSystemModel;
    QFileInfo fi(archiveDirectory);
    filemodel->setRootPath(fi.canonicalFilePath());
    QStringList filter("*.sym");
    filemodel->setNameFilters(filter);
    filemodel->setNameFilterDisables(false);

    treeView->setModel(filemodel);
    treeView->setRootIndex(filemodel->index(fi.canonicalFilePath()));

    treeView->selectionModel()->setCurrentIndex(filemodel->index(fi.canonicalFilePath()),QItemSelectionModel::Select);
    treeView->expand(filemodel->index(fi.canonicalFilePath()));
    filemodel->setRootPath(fi.canonicalFilePath());
    QHeaderView* header = treeView->header();
    header->hideSection(1);
    header->hideSection(2);
    //header->hideSection(3);
    header->resizeSection(0,300);
}

void MainWindow::treeView_doubleClicked(QModelIndex index)
{
    qDebug() << treeView->selectionModel()->currentIndex().data().toString();
    if(!index.isValid())
        return;
    QFileSystemModel* fileModel = qobject_cast<QFileSystemModel*>(treeView->model());
    if(fileModel->isDir(index) || fileModel->fileInfo(index).suffix() != "sym")
        return;

    scene->addItem( Parser::toComponent(fileModel->filePath(index)));
}

void MainWindow::slider_valueChanged(int value)
{
    double newScale = QString::number(value).toDouble() ;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

void MainWindow::scene_selectionChanged()
{

}
