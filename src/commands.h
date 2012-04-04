/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   22/03/2012
****************************************************************************/

#ifndef COMMANDS_H
#define COMMANDS_H

#include "graphicsscene.h"

#include <QUndoCommand>
#include <QGraphicsItem>

class MoveCommand : public QUndoCommand
{
public:
    enum { Id = 1234 };

    MoveCommand(QGraphicsItem *diagramItem, const QPointF &oldPos,
                QUndoCommand *parent = 0);

    void undo();
    void redo();
//    bool mergeWith(const QUndoCommand *command);
    int id() const { return Id; }

private:
    QGraphicsItem *myDiagramItem;
    QPointF myOldPos;
    QPointF newPos;
};

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(QList<QGraphicsItem*> items,GraphicsScene *graphicsScene, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    QList<QGraphicsItem*> myDiagramItems;
    GraphicsScene *myGraphicsScene;
};

class AddCommand : public QUndoCommand
{
public:
    AddCommand(QGraphicsItem* graphicsItem, GraphicsScene *graphicsScene, QUndoCommand *parent = 0);
    ~AddCommand();

    void undo();
    void redo();

private:
    QGraphicsItem *myDiagramItem;
    GraphicsScene *myGraphicsScene;
    QPointF initialPosition;
};

//QString createCommandString(QGraphicsItem *item, const QPointF &point);

#endif // COMMANDS_H
