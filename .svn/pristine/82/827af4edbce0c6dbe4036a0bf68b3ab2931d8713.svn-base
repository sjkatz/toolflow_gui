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
#include <QGraphicsLineItem>

class Wire : public QGraphicsLineItem
{
public:

    enum { Type = /*UserType +*/ 457 };

    Wire(Port* source, Port* destination, int id = 0,
         QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    Wire(Wire& ref);

    int type() const
        { return Type; }
    QRectF boundingRect() const;

    Port* source();
    void setSource(Port* source);
    Port* destination();
    void setDestination(Port* destination);
    int id();
    void setId(int id);
//    QPen pen();
//    void setPen(QPen pen);

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    Port* d_source;
    Port* d_destination;
    int d_id;
  //  QPen d_pen;
};

#endif // WIRE_H
