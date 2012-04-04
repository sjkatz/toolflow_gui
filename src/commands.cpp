/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   22/03/2012
****************************************************************************/

#include <QtGui>

#include "commands.h"

MoveCommand::MoveCommand(QGraphicsItem *diagramItem, const QPointF &oldPos,
                         QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myDiagramItem = diagramItem;
    newPos = diagramItem->pos();
    myOldPos = oldPos;
}

//bool MoveCommand::mergeWith(const QUndoCommand *command)
//{
//    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
//    QGraphicsItem *item = moveCommand->myDiagramItem;

//    if (myDiagramItem != item)
//        return false;

//    newPos = item->pos();
////    setText(QObject::tr("Move %1")
////            .arg(createCommandString(myDiagramItem, newPos)));

//    return true;
//}

void MoveCommand::undo()
{
    myDiagramItem->setPos(myOldPos);
    myDiagramItem->scene()->update();
//    setText(QObject::tr("Move %1")
//            .arg(createCommandString(myDiagramItem, newPos)));
}

void MoveCommand::redo()
{
    myDiagramItem->setPos(newPos);
//    setText(QObject::tr("Move %1")
//            .arg(createCommandString(myDiagramItem, newPos)));
}

DeleteCommand::DeleteCommand(QList<QGraphicsItem*> items, GraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = scene;
//    QList<QGraphicsItem *> list = myGraphicsScene->selectedItems();
//    list.first()->setSelected(false);
//    myDiagramItem = list.first();
    myDiagramItems = items;
//    setText(QObject::tr("Delete %1")
//            .arg(createCommandString(myDiagramItem, myDiagramItem->pos())));
}

void DeleteCommand::undo()
{
    foreach(QGraphicsItem* item , myDiagramItems)
    {
        //myGraphicsScene->addItem(myDiagramItem);
        if(Component* comp = qgraphicsitem_cast<Component*>(item))
            myGraphicsScene->addComponent(comp);
        else if(Wire* wire = qgraphicsitem_cast<Wire*>(item))
            myGraphicsScene->addWire(wire);
        else
            myGraphicsScene->addItem(item);
    }

    myGraphicsScene->update();
}

void DeleteCommand::redo()
{
   // myGraphicsScene->removeItem(myDiagramItem);

    foreach(QGraphicsItem* item , myDiagramItems)
    {
        if(Component* comp = qgraphicsitem_cast<Component*>(item))
            myGraphicsScene->removeComponent(comp);
        else if(Wire* wire = qgraphicsitem_cast<Wire*>(item))
            myGraphicsScene->removeWire(wire);
        else
            myGraphicsScene->removeItem(item);
    }
}


AddCommand::AddCommand(QGraphicsItem* item, GraphicsScene *scene, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myGraphicsScene = scene;
    myDiagramItem = item;
    initialPosition = item->scenePos();
    scene->update();
//    setText(QObject::tr("Add %1")
//            .arg(createCommandString(myDiagramItem, initialPosition)));
}

AddCommand::~AddCommand()
{
    if (!myDiagramItem->scene())
        delete myDiagramItem;
}

void AddCommand::undo()
{
//    myGraphicsScene->removeItem(myDiagramItem);

    if(Component* comp = qgraphicsitem_cast<Component*>(myDiagramItem))
        myGraphicsScene->removeComponent(comp);
    else if(Wire* wire = qgraphicsitem_cast<Wire*>(myDiagramItem))
        myGraphicsScene->removeWire(wire);
    else
        myGraphicsScene->removeItem(myDiagramItem);

    myGraphicsScene->update();
}

void AddCommand::redo()
{
    if(Component* comp = qgraphicsitem_cast<Component*>(myDiagramItem))
        myGraphicsScene->addComponent(comp);
    else if(Wire* wire = qgraphicsitem_cast<Wire*>(myDiagramItem))
        myGraphicsScene->addWire(wire);
    else
        myGraphicsScene->addItem(myDiagramItem);

//    myGraphicsScene->addItem(myDiagramItem);
//    myDiagramItem->setPos(initialPosition);
    myGraphicsScene->clearSelection();
    myGraphicsScene->update();
}

//QString createCommandString(QGraphicsItem *item, const QPointF &pos)
//{
//    return QObject::tr("%1 at (%2, %3)")
//            .arg(item->diagramType() == DiagramItem::Box ? "Box" : "Triangle")
//            .arg(pos.x()).arg(pos.y());
//}
