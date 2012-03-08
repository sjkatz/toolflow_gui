/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#ifndef COMPONENT_H
#define COMPONENT_H

#include "port.h"
#include <QList>
#include <QGraphicsItemGroup>
#include <QPainter>

class Component : public QGraphicsItemGroup
{
public:
    enum { Type = /*UserType + */256 };

    Component(QString name, QList<Port*> ports = QList<Port*>(), int id = 0, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    //Component(Component* ref);
    ~Component();

    int type() const
        { return Type; }

    int id();
    void setId(int id);
    QList<Port*> ports();
    void setPorts(QList<Port*> ports);
    QString name();
    void setName(QString& name);
    QRectF rect();
    void setRect(QRectF rect);

    void addPort(Port& port, int x = 0, int y = 0);

private:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    QRectF boundingRect() const;


private:
    int d_id;
    QList<Port*> d_ports;
    QString d_name;
    QRectF d_rect;
    QGraphicsLineItem* d_line;
};

#endif // COMPONENT_H
