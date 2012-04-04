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
#include "commands.h"

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
    connect(ui->treeProcessWidget,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(treeProcessWidget_doubleClicked(QModelIndex)));
    connect(ui->verticalSlider,SIGNAL(valueChanged(int)), this, SLOT(slider_valueChanged(int)));


    QSplitter *splitterMain = new QSplitter(ui->mainWidget_Container);
    splitterMain->setOrientation(Qt::Vertical);
    QSplitter *splitterV = new QSplitter();
    splitterV->setOrientation(Qt::Vertical);
    splitterV->addWidget(treeView);
    splitterV->addWidget(ui->treeProcessWidget);
    QSplitter *splitterH = new QSplitter();
    //splitterH->addWidget(treeView);
    splitterH->addWidget(splitterV);
    splitterH->addWidget(view);
    splitterMain->addWidget(splitterH);
    splitterMain->addWidget(ui->txtReport);
    QGridLayout* layout = new QGridLayout(ui->mainWidget_Container);
    layout->addWidget(splitterMain);
    splitterH->setStretchFactor(1,1);
    //splitterV->setStretchFactor(0,3);
    splitterMain->setStretchFactor(1,1);
    layout->setMargin(0);


}

MainWindow::~MainWindow()
{
    delete treeView;
    delete action_open;
    delete action_export;
    delete action_save;
    delete action_delete;
    delete action_wire;
    delete action_select;
    delete action_undo;
    delete action_redo;
    delete action_execute;
    delete undo_stack;
    delete scene;
    delete ui;
}

void MainWindow::setupToolBar()
{
    ui->mainToolBar->addWidget(new QToolBar);

    action_open = ui->mainToolBar->addAction(QIcon(ICON_OPEN),"Open");
    connect(action_open,SIGNAL(triggered()), this, SLOT(actionOpen_Triggered()));

    action_save = ui->mainToolBar->addAction(QIcon(ICON_SAVE),"Save");
    connect(action_save,SIGNAL(triggered()), this, SLOT(actionSave_Triggered()));

    action_export = ui->mainToolBar->addAction(QIcon(ICON_EXPORT),"Export");
    connect(action_export,SIGNAL(triggered()), this, SLOT(actionExport_Triggered()));

    ui->mainToolBar->addSeparator();
    undo_stack = new QUndoStack(this);
    action_undo = undo_stack->createUndoAction(this, tr("&Undo"));
    action_undo->setShortcuts(QKeySequence::Undo);
    action_undo->setIcon(QIcon(ICON_UNDO));
    ui->mainToolBar->addAction(action_undo);

    action_redo = undo_stack->createRedoAction(this, tr("&Redo"));
    action_redo->setShortcuts(QKeySequence::Redo);
    action_redo->setIcon(QIcon(ICON_REDO));
    ui->mainToolBar->addAction(action_redo);

    connect(undo_stack, SIGNAL(cleanChanged(bool)),this, SLOT(undoStack_CleanChanged(bool)));

    ui->mainToolBar->addSeparator();

    action_wire = ui->mainToolBar->addAction(QIcon(ICON_WIRE),"Wire");
    action_wire->setCheckable(true);
    connect(action_wire,SIGNAL(triggered()), this, SLOT(actionWire_Triggered()));

    action_select = ui->mainToolBar->addAction(QIcon(ICON_SELECT),"Select");
    action_select->setCheckable(true);
    action_select->setChecked(true);
    connect(action_select,SIGNAL(triggered()), this, SLOT(actionSelect_Triggered()));

    ui->mainToolBar->addSeparator();

    action_execute = ui->mainToolBar->addAction(QIcon(ICON_EXECUTE),"Execute");
    connect(action_execute,SIGNAL(triggered()), this, SLOT(actionExecute_Triggered()));
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
//     for(int x = 0; x <= 1000; x = x + 20) {
//         for(int y = 0; y <= 1000; y = y + 20) {
//             //scene->addRect(x, y, 24, 24,pen);
//            //scene->addLine(x,y,x,1000,Qt::DashDotDotLine);
//            //scene->addLine(x,y,1000,y,Qt::DashDotDotLine);
//             if(x % 100 == 0 && y % 100 == 0) {
//                 scene->addRect(x, y, 2, 2);
//                 //scene->addLine(x,0,x,1000,penpage);
//                 //scene->addLine(0,y,1000,y,penpage);
//                // painter->drawLine(0,y,1000,y);

//                 QString pointString;
//                 QTextStream stream(&pointString);
//                 stream << "(" << x << "," << y << ")";
//                 //QGraphicsTextItem* item = scene->addText(pointString);
//                 //item->setPos(x, y);
//             } else {
//                 scene->addRect(x, y, 1,1);
//                 //scene->addLine(0,y+5,1000,y+5,pen);
//                 //scene->addLine(x,0,x,1000,pengrid);
//                 //scene->addLine(0,y,1000,y,pengrid);

//             }
//         }

//     }
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
     connect(scene, SIGNAL(itemMoved(QGraphicsItem*,QPointF)), this, SLOT(itemMoved(QGraphicsItem*,QPointF)));
     connect(scene, SIGNAL(addGraphicsItem(QGraphicsItem*)), this, SLOT(addGraphicsItem(QGraphicsItem*)));
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

    Parser::fromXML(fileName,scene);
}

void MainWindow::actionSave_Triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Export File"),
            QApplication::applicationDirPath()+"/../",
            tr("Extensible Markup Language(*.xml)") );

    if(fileName.isEmpty())
        return;

    Parser::toXML(fileName,scene);
    setWindowModified(false);
}

void MainWindow::actionExport_Triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
            this,
            tr("Export File"),
            QApplication::applicationDirPath()+"/../",
            tr("Microprocessor Hardware Specification(*.mhs)") );

    if(fileName.isEmpty())
        return;
    Parser::toMHS(fileName,scene);
}

void MainWindow::actionExecute_Triggered(QStringList args)
{
    /* create QProcess object */
    proc = new QProcess();
    proc->setWorkingDirectory("C:/Users/Shaun/Desktop/tests/test6/");

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//    env.insert("XILINX_EDK","C:\\Xilinx\\13.4\\ISE_DS\\EDK"); // Add an environment variable
    env.insert("PATH", env.value("Path") + ";C:\\Xilinx\\13.4\\ISE_DS\\EDK\\bin\\nt64");
    proc->setProcessEnvironment(env);


    if(args.isEmpty())
        args << "-s" << "-i" << "-g";

    proc->start("C:\\Python23\\python.exe", QStringList() << QString("xps_gen.py") << args);
    //proc->start("cmd", QStringList() << "/k" << QString("xps_gen"));

    /* show output */
    connect(proc, SIGNAL(readyReadStandardOutput()),this, SLOT(rightMessage()) );
    connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(wrongMessage()) );

}

// show right message
void MainWindow::rightMessage()
{
    QByteArray strdata = proc->readAllStandardOutput();
    ui->txtReport->setTextColor(Qt::black);
    ui->txtReport->append(strdata);
}

// show wrong message
void MainWindow::wrongMessage()
{
    QByteArray strdata = proc->readAllStandardError();
    ui->txtReport->setTextColor(Qt::red);
    ui->txtReport->append(strdata);
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
//    foreach(QGraphicsItem* item , scene->selectedItems())
//    {
//        if(Component* comp = qgraphicsitem_cast<Component*>(item))
//            scene->removeComponent(comp);
//        else if(Wire* wire = qgraphicsitem_cast<Wire*>(item))
//            scene->removeWire(wire);
//        else
//            scene->removeItem(item);
//    }


    if (scene->selectedItems().isEmpty())
             return;

    QUndoCommand *deleteCommand = new DeleteCommand(scene->selectedItems(), scene);
    undo_stack->push(deleteCommand);

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
    if(!index.isValid())
        return;
    QFileSystemModel* fileModel = qobject_cast<QFileSystemModel*>(treeView->model());
    if(fileModel->isDir(index) || fileModel->fileInfo(index).suffix() != "sym")
        return;

    //scene->addComponent(Parser::toComponent(fileModel->filePath(index)));
    Component* comp1 = Parser::toComponent(fileModel->filePath(index));
    addGraphicsItem(comp1);
    comp1->scale(5,5);
    comp1->setPos(scene->width()/2,scene->height()/2);
}

void MainWindow::treeProcessWidget_doubleClicked(QModelIndex index)
{
    if(!index.isValid())
        return;
    int row = index.row();
    QStringList args;
    switch(row)
    {
        case 2:{    //GENERATE
            args << "-g";
        }
        case 1:{    //IMPLEMENT
            args << "-i";
        }
        case 0:{    //SYNTHESIS
            args << "-s";
            break;
        }
        default:
        {
            args << "-g -i -s";
        }
    }
    //qDebug() << args;
    actionExecute_Triggered(args);

}

void MainWindow::slider_valueChanged(int value)
{
    double newScale = QString::number(value).toDouble() ;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

void MainWindow::itemMoved(QGraphicsItem* item,QPointF old_pos)
{
    undo_stack->push(new MoveCommand(item, old_pos));
}

void MainWindow::addGraphicsItem(QGraphicsItem* item )
{
    undo_stack->push(new AddCommand(item,scene));
}

void MainWindow::undoStack_CleanChanged(bool is_clean)
{
    setWindowModified(!is_clean);
}
