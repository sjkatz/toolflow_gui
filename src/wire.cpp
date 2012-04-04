/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#include "wire.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>
#include <math.h>

const qreal Pi = 3.14;

Wire::Wire(Port* source, Port* destination, QString id, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPathItem(parent, scene)
{
    setSource(source);
    setDestination(destination);
    setId(id);

    QPointF src = mapFromItem(d_source, 0, 0);
    QPointF dest = mapFromItem(d_destination, 0, 0);
    d_half = src + (dest-src)/2;

    segment = new QGraphicsRectItem(this);
    segment->setFlag(QGraphicsItem::ItemIsMovable, true);
    segment->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    segment->setFlag(QGraphicsItem::ItemIsSelectable, true);
    segment->setRect(-2,0,4,4);
    segment->setBrush(QBrush(Qt::black));
    //segment->setLine(d_half.x(),src.y(),d_half.x(),dest.y());
    //this->scene()->addItem(segment);
    //scene->addItem(segment);

    QPointF half = mapToScene(d_half);
    segment->setPos(half);

    //setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    updatePosition();
}

Wire::Wire(Wire& ref)
    : QGraphicsPathItem(ref.parentItem(), ref.scene())
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

QString Wire::id()
{
    return d_id;
}

void Wire::setId(QString id)
{
    d_id = id;
}

QPointF Wire::half()
{
    return d_half;
}

void Wire::setHalf(QPointF half)
{
    d_half = half;
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
    qreal extra = (pen().width());

    return  QGraphicsPathItem::boundingRect().normalized()
            .adjusted(-extra, -extra, extra, extra);

//    return polygon().boundingRect().normalized()
//            .adjusted(-extra, -extra, extra, extra);

//    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
//                                      line().p2().y() - line().p1().y()))
//        .normalized()
//        .adjusted(-extra, -extra, extra, extra);
}

void Wire::updatePosition()
{
    //if(polygon().isEmpty())
    //    return;
    //polygon().pop_front();
    //polygon().pop_back();


//    QPolygonF poly;
//    poly << mapFromItem(d_source, 0, 0);

//    foreach(QPointF point, polygon())
//    {
//        //qDebug() << point.x() << point.y();
//        if(point != polygon().front() && point != polygon().back())
//            poly << point;

//    }

//    poly << mapFromItem(d_destination, 0, 0);
//    setPolygon(poly);
    //lines.clear();
    QLineF line(mapFromItem(d_source, 0, 0), mapFromItem(d_destination, 0, 0));
    QPointF src = mapFromItem(d_source, 0, 0);
    QPointF dest = mapFromItem(d_destination, 0, 0);
    //d_half = src + (dest-src)/2;
    //QPointF oldHalf = mapToItem(segment,d_half);

    //src.rx()-=20*source()->posFactor();
   // dest.rx()+=2*(1/destination()->posFactor());
   // QPointF half = src + (dest-src)/2;




    QPainterPath path(src);
    if(src.x() > dest.x()){
        QPointF newHalf = mapToScene(src + (dest-src)/2);
        segment->setPos(newHalf.x(),segment->y());
        QPointF seg = mapFromItem(segment, 0, 0);
        path.lineTo( QPointF(src.x(),seg.y()));
        path.lineTo(QPointF(dest.x(),seg.y()));
        path.lineTo(dest);
    }else{
        QPointF newHalf = mapToScene(src + (dest-src)/2);
        segment->setPos(segment->x(),newHalf.y());
        QPointF seg = mapFromItem(segment, 0, 0);
        path.lineTo(QPointF(seg.x(),src.y()));
        path.lineTo(QPointF(seg.x(),dest.y()));
        path.lineTo(dest);
    }

    setPath(path);


//    QPolygonF poly;

//    if(source.x() > dest.x())
//        poly << source << QPointF(source.x(),half.y())
//                 << QPointF(dest.x(),half.y()) << dest;
//    else
//        poly << source << QPointF(half.x(),source.y())
//                 << QPointF(half.x(),dest.y()) << dest;

//    setPolygon(poly);
    //QLineF linex(line.p1(), QPointF(line.x1()+line.dx(),line.y1()));
    //QLineF liney(linex.p2(), QPointF(linex.x2(),line.y1()+line.dy()));
    //lines.append(linex);
    //lines.append(liney);
    //setLine(line);
}

void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    if(isSelected())
        setPen(QPen(Qt::red));
    else
        setPen(QPen(Qt::black));
    painter->setPen(pen());
    //painter->drawLines(lines);
//    foreach(QLineF line, lines){
//        qDebug() << line.x1() << line.y1();
//        painter->drawLine(line);
//    }
    //painter->drawLine(line());
    //painter->drawPolyline(polygon());
    painter->drawPath(path());
    updatePosition();
}

QPainterPath Wire::shape() const{
    QPainterPath path(this->path());
    QPainterPathStroker pathStrocker;
    return pathStrocker.createStroke(path);
}

