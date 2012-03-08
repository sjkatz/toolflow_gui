/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#include "wire.h"
#include <QPainter>

#include <math.h>

const qreal Pi = 3.14;

Wire::Wire(Port* source, Port* destination, int id, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    setSource(source);
    setDestination(destination);
    setId(id);

    //setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

Wire::Wire(Wire& ref)
    : QGraphicsLineItem(ref.parentItem(), ref.scene())
{
    setSource(ref.source());
    setDestination(ref.destination());
    setId(ref.id());
}

Port* Wire::source()
{
    return d_source;
}

void Wire::setSource(Port* source)
{
    d_source = source;
}

Port* Wire::destination()
{
    return d_destination;
}

void Wire::setDestination(Port* destination)
{
    d_destination = destination;
}

int Wire::id()
{
    return d_id;
}

void Wire::setId(int id)
{
    d_id = id;
}

//QPen Wire::pen()
//{
//    return d_pen;
//}

//void Wire::setPen(QPen pen)
//{
//    d_pen = pen;
//    update();
//}

QRectF Wire::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Wire::updatePosition()
{
    QLineF line(mapFromItem(d_source, 0, 0), mapFromItem(d_destination, 0, 0));
    setLine(line);
}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if(isSelected())
        setPen(QPen(Qt::blue));
    else
        setPen(QPen(Qt::black));
    painter->setPen(pen());
    painter->drawLine(line());
    updatePosition();
}

