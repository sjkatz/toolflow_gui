/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#ifndef WIRE_H
#define WIRE_H

#include "port.h"
#include <QGraphicsPathItem>

class Wire : public QGraphicsPathItem
{
public:

    enum { Type = /*UserType +*/ 457 };

    Wire(Port* source, Port* destination, QString id = QString(), QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    Wire(Wire& ref);

    int type() const
        { return Type; }
    QRectF boundingRect() const;

    /**
    Returns the shape of this item as a QPainterPath in local coordinates.
    The shape is used for many things, including collision detection, hit tests, and for the QGraphicsScene::items() functions.
      */
    QPainterPath shape() const;


    Port* source();
    void setSource(Port* source);
    Port* destination();
    void setDestination(Port* destination);
    QString id();
    void setId(QString id);
//    QPen pen();
//    void setPen(QPen pen);
    QPointF half();
     void setHalf(QPointF half);

    void updatePosition();
    QGraphicsRectItem* segment;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);


private:
    Port* d_source;
    Port* d_destination;
    QString d_id;
    QVector<QLineF> lines;
  //  QPen d_pen;
    QPointF d_half;

};

#endif // WIRE_H
