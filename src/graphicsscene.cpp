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

GraphicsScene::GraphicsScene( QObject *parent)
    : QGraphicsScene(parent)
{
    myMode = MoveItem;
    d_line = 0;
}

void GraphicsScene::setMode(Mode mode)
{
    myMode = mode;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::MiddleButton){
        last_center_point = mouseEvent->scenePos();
        //QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    }
    else if (mouseEvent->button() != Qt::LeftButton)
        return;

    if(myMode == InsertLine){
        d_line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
        //line->setPen(QPen(myLineColor, 2));
        addItem(d_line);
    }else
        QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
   // qDebug() << mouseEvent->button();
    //if (mouseEvent->button() == Qt::MiddleButton){
        //if(!last_center_point.isNull()){
//            qDebug() << last_center_point.x() << last_center_point.y();
//            QPointF delta = last_center_point - mouseEvent->scenePos();
//            last_center_point = mouseEvent->scenePos();
//            views().front()->centerOn(last_center_point + delta);
//            qDebug() << last_center_point.x() << last_center_point.y();
//            qDebug() << delta.x() << delta.y();
        //}
   // }

    if (d_line != 0 && myMode == InsertLine) {
        QApplication::setOverrideCursor(Qt::CrossCursor);
        QLineF newLine(d_line->line().p1(), mouseEvent->scenePos());
        d_line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::MiddleButton){
        last_center_point = QPointF();
       // QApplication::setOverrideCursor(Qt::OpenHandCursor);
    }

    if (d_line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(d_line->line().p1());
        if (startItems.count() && startItems.first() == d_line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(d_line->line().p2());
        if (endItems.count() && endItems.first() == d_line)
            endItems.removeFirst();

        removeItem(d_line);
        delete d_line;

        if(startItems.count() && endItems.count()){
            if(Port *startItem = qgraphicsitem_cast<Port *>(startItems.first())){
                if(Port *endItem = qgraphicsitem_cast<Port *>(endItems.first())){
                    Wire* wire = new Wire(startItem, endItem);
                    addItem(wire);
                    wire->updatePosition();
                }
            }
        }
    }
    d_line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    foreach(QUrl url, event->mimeData()->urls()){
        Component* comp = Parser::toComponent(url.toLocalFile());
        if(!comp)
            break;
        addItem(comp);
        QPointF point = event->scenePos();
        comp->translate(point.x(),point.y());
        comp->scale(5,5);
    }
}

void GraphicsScene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{

}



