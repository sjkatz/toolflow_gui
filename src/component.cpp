/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#include "component.h"
#include "wire.h"
#include "propertydialog.h"
#include "propertymodel.h"

#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QWidget>
#include <QApplication>
#include <QGraphicsScene>
#include <QPointer>
#include <QGraphicsItem>

Component::Component(QString name, QList<Port*> ports, QString id, QGraphicsItem *parent, QGraphicsScene *scene)
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

    d_port_count = 0;

}

//Component::Component(Component* ref)
//{
//    setName(ref->name());
//    setPorts(ref->ports());
//    setId(ref->id());
//}

Component::~Component()
{
}

QString Component::id()
{
    return d_id;
}

void Component::setId(QString id)
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

QMap<QString,QVariant> Component::data()
{
    return d_data;
}

void Component::setData(QMap<QString,QVariant> data)
{
    d_data = data;
}

void Component::addPort(Port& port, int x, int y)
{
    if(port.id().isEmpty()){
        d_port_count++;
        port.setId(QString("P%1").arg(d_port_count));
    }
    else
    {
        int portID = port.id().split("P").last().toInt();
        if(d_port_count < portID)
            d_port_count = portID;
    }
    port.setPos(x,y);
    d_ports.append(&port);
    addToGroup(&port);

}

Port* Component::findPort(QString id)
{
    foreach(Port* port, d_ports)
    {
        if(port->id() == id)
            return port;
    }

    return 0;
}

void Component::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    if(isSelected()){
        d_pen = QPen(Qt::red,0.5);

        QList<QGraphicsItem *> overlapItems = collidingItems();

        qreal zValue = 0;
        foreach (QGraphicsItem *item, overlapItems) {
            if (item->zValue() >= zValue &&
                item->type() == Component::Type)
                zValue = item->zValue() + 0.1;
        }
        setZValue(zValue);

    }else{
        d_pen = QPen(Qt::black,0.5);
    }
    painter->setPen(d_pen);
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(d_rect);

//    painter->setRenderHint(QPainter::Antialiasing,true);
//    painter->setRenderHint(QPainter::TextAntialiasing,true);
//    painter->setRenderHint(QPainter::SmoothPixmapTransform,true);

    QRectF rect(d_rect.x(), d_rect.y()-2.5,d_rect.width(),2);

    QFont font = QApplication::font();
    font.setPixelSize( rect.height() );
    font.setLetterSpacing(QFont::PercentageSpacing,140);
    painter->setFont( font );
    painter->drawText(rect, Qt::AlignCenter, name());
}

QRectF Component::boundingRect() const
{
    qreal extra = (d_pen.width());

    return QRectF(d_rect.x(), d_rect.y()-2.5, d_rect.width(), d_rect.height()+2.5).normalized()
            .adjusted(-extra, -extra, extra, extra);;
}

void Component::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent  * event )
{
    PropertyDialog* propertyDialog = new PropertyDialog(this);
    if(propertyDialog->exec()){
        PropertyModel* model = qobject_cast<PropertyModel*>(propertyDialog->view()->model());
        d_data = model->propertyMap();
    }
}




