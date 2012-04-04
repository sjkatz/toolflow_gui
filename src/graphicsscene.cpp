/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   06/03/2012
****************************************************************************/

#include <QtGui>
#include <QFile>

#include "graphicsscene.h"
#include "wire.h"
#include "parser.h"
//#include "commands.h"

GraphicsScene::GraphicsScene( QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;
    d_line = 0;
    d_wire_count = 0;
    d_component_count = 0;
    d_moving_item = 0;
}

QList<Component*> GraphicsScene::components()
{
    return d_components;
}

void GraphicsScene::addComponent(Component* component)
{
    if(component->id().isEmpty()){
        d_component_count++;
        component->setId(QString("C%1").arg(d_component_count));
    }
    else
    {
        int compID = component->id().split("C").last().toInt();
        if(d_component_count < compID)
            d_component_count = compID;
    }
    addItem(component);
    d_components.removeAll(component);
    d_components.append(component);
}

void GraphicsScene::removeComponent(Component* component)
{
    removeItem(component);
    d_components.removeAll(component);
}

Component* GraphicsScene::findComponent(QString id)
{
    foreach(Component* comp, d_components)
    {
        if(comp->id() == id)
            return comp;
    }

    return 0;
}

QList<Wire*> GraphicsScene::wires()
{
    return d_wires;
}

void GraphicsScene::addWire(Wire* wire)
{
    if(wire->id().isEmpty()){
        d_wire_count++;
        wire->setId(QString("W%1").arg(d_wire_count));
    }
    else
    {
        int wireID = wire->id().split("W").last().toInt();
        if(d_wire_count < wireID)
            d_wire_count = wireID;
    }
    addItem(wire);
    d_wires.removeAll(wire);
    d_wires.append(wire);
    wire->updatePosition();
}

void GraphicsScene::removeWire(Wire* wire)
{
    removeItem(wire);
    d_wires.removeAll(wire);
}

void GraphicsScene::setWireStart(QPointF point) {
    d_line = new QGraphicsLineItem(QLineF(point,point));
    d_line->setPen(QPen(QBrush(Qt::black), 2,Qt::DashLine));
    addItem(d_line);
}

void GraphicsScene::setWireStop(QPointF point) {
    if (d_line != 0)
        d_line->setLine(QLineF(d_line->line().p1(), point));
}

void GraphicsScene::setWire() {
    if (d_line == 0)
        return;
    QList<QGraphicsItem *> startItems = items(d_line->line().p1());
    if (startItems.count() && startItems.first() == d_line)
        startItems.removeFirst();
    QList<QGraphicsItem *> endItems = items(d_line->line().p2());
    if (endItems.count() && endItems.first() == d_line)
        endItems.removeFirst();

    removeItem(d_line);
    delete d_line;

    if(startItems.count() && endItems.count()){
        if(QGraphicsEllipseItem *startItem = qgraphicsitem_cast<QGraphicsEllipseItem *>(startItems.first())){
            if(Port *endItem = qgraphicsitem_cast<Port *>(endItems.first())){
                Wire* wire = new Wire(qgraphicsitem_cast<Port *>(startItem->parentItem()),endItem);
                //addWire(wire);
                emit addGraphicsItem(wire);
                //wire->updatePosition();
            }
        }
    }
    d_line = 0;
}

void GraphicsScene::setMode(Mode mode)
{
    myMode = mode;
    if(myMode == MoveItem){
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }else{
        QApplication::setOverrideCursor(Qt::CrossCursor);
    }
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    setWire();
    if (mouseEvent->button() != Qt::LeftButton || mouseEvent->modifiers() == Qt::AltModifier)
        return;

    if(qgraphicsitem_cast<QGraphicsEllipseItem*>(itemAt(mouseEvent->scenePos())))
        setMode(InsertLine);
    else
        setMode(MoveItem);

    if(myMode == MoveItem){
        d_moving_item = itemAt(mouseEvent->scenePos());

        if (d_moving_item != 0) {
            d_old_item_pos = d_moving_item->pos();
        }

        //clearSelection();
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

//void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
//{
//    QGraphicsScene::mouseMoveEvent(mouseEvent);
//    if (d_line != 0 && myMode == InsertLine) {
//        //QApplication::setOverrideCursor(Qt::CrossCursor);
//        QLineF newLine(d_line->line().p1(), mouseEvent->scenePos());
//        d_line->setLine(newLine);
//    } else if (myMode == MoveItem) {
//        //QApplication::setOverrideCursor(Qt::ArrowCursor);
//        //QGraphicsScene::mouseMoveEvent(mouseEvent);
//    }
//}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton)
        return;

    if(myMode == MoveItem){
        if (d_moving_item != 0) {
            if (d_old_item_pos != d_moving_item->pos())
                emit itemMoved(d_moving_item,d_old_item_pos);
                d_moving_item = 0;
            }
    }
    setMode(MoveItem);
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{

    foreach(QUrl url, event->mimeData()->urls()){
        Component* comp = Parser::toComponent(url.toLocalFile());
        if(!comp)
            break;
        //addItem(comp);
        //addComponent(comp);
        emit addGraphicsItem(comp);
        QPointF point = event->scenePos();
        //comp->translate(point.x(),point.y());
        comp->setPos(point.x(),point.y());
        comp->scale(5,5);
    }
}

void GraphicsScene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
    //QGraphicsScene::dragMoveEvent(event);
}



