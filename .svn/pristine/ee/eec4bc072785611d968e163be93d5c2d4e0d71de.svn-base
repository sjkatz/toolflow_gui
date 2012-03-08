/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#include "component.h"
#include "wire.h"

#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QWidget>
#include <QApplication>
#include <QGraphicsScene>
#include <QPointer>
#include <QGraphicsItem>

Component::Component(QString name, QList<Port*> ports, int id, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItemGroup(parent,scene)
{
    setName(name);
    setPorts(ports);
    setId(id);
    d_rect = QRectF(0, 0, 20, 20);

    setFlag(QGraphicsItemGroup::ItemIsMovable, true);
    setFlag(QGraphicsItemGroup::ItemIsSelectable, true);
    setFlag(QGraphicsItemGroup::ItemSendsGeometryChanges, true);
    setHandlesChildEvents(false);
    //setAcceptHoverEvents(true);

}

//Component::Component(Component* ref)
//{
//    setName(ref->name());
//    setPorts(ref->ports());
//    setId(ref->id());
//}
Component::~Component()
{
    //delete d_line;
}

int Component::id()
{
    return d_id;
}

void Component::setId(int id)
{
    d_id = id;
}

QList<Port*> Component::ports()
{
    return d_ports;
}

void Component::setPorts(QList<Port*> ports)
{
    d_ports = ports;
}

QString Component::name()
{
    return d_name;
}

void Component::setName(QString& name)
{
    d_name = name;
}

QRectF Component::rect()
{
    return d_rect;
}

void Component::setRect(QRectF rect)
{
    d_rect = rect;
}

void Component::addPort(Port& port, int x, int y)
{
    port.translate(x,y);
    d_ports.append(&port);
    addToGroup(&port);

}

void Component::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    QPen pen;
    if(isSelected()){
        pen = QPen(Qt::blue);

        QList<QGraphicsItem *> overlapItems = collidingItems();

        qreal zValue = 0;
        foreach (QGraphicsItem *item, overlapItems) {
            if (item->zValue() >= zValue &&
                item->type() == Component::Type)
                zValue = item->zValue() + 0.1;
        }
        setZValue(zValue);

    }else{
        pen = QPen(Qt::black);
    }
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(d_rect);

    QRectF rect(d_rect.x(), d_rect.y()-2.5,d_rect.width(),2);
    QFont font = QApplication::font();
    font.setPixelSize( rect.height() );
    painter->setFont( font );
    painter->drawText(rect, Qt::AlignCenter, name());
}

QRectF Component::boundingRect() const
{

    return QRectF(d_rect.x(), d_rect.y()-2.5, d_rect.width(), d_rect.height()+2.5);
}




