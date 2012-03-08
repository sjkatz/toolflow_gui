/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#include "port.h"
#include <QPen>
#include <QDebug>
#include <QPainter>
#include <QApplication>

Port::Port(QString name, Direction direction, DataType type, int msb, int lsb, int id, QGraphicsItem * parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene)
{
    setName(name);
    setDirection(direction);
    setDataType(type);
    setMsb(msb);
    setLsb(lsb);
    setId(id);
    d_pen = QPen(Qt::black);
    //d_anchor = QRectF(-0.5,-0.5,1,1);

    setAcceptHoverEvents(true);
}

Port::Port(Port& ref)
    : QGraphicsItem(ref.parentItem(), ref.scene())
{

    setName(ref.name());
    setDirection(ref.direction());
    setDataType(ref.dataType());
    setMsb(ref.msb());
    setLsb(ref.lsb());
    setId(ref.id());
}

Port& Port::operator=(Port& ref)
{
    Port* port = new Port(ref);
    return *port;
}

int Port::id()
{
    return d_id;
}

void Port::setId(int id)
{
    d_id = id;
}

QString Port::name()
{
    return d_name;
}

void Port::setName(QString name)
{
    d_name = name;
}

Port::Direction Port::direction()
{
    return d_direction;
}

void Port::setDirection(Direction direction)
{
    d_direction = direction;
}

Port::DataType Port::dataType()
{
    return d_type;
}

void Port::setDataType(DataType type)
{
    d_type = type;
}

int Port::msb()
{
    return d_msb;
}

void Port::setMsb(int msb)
{
    d_msb = msb;
}

int Port::lsb()
{
    return d_lsb;
}

void Port::setLsb(int lsb)
{
    d_lsb = lsb;
}

int Port::width()
{
    return d_width;
}

void Port::setWidth(int width)
{
    d_width = width;
}

QPen Port::pen()
{
    return d_pen;
}

void Port::setPen(QPen pen)
{
    d_pen = pen;
    update();
}

QLineF Port::line()
{
    return d_line;
}

void Port::setLine(QLineF line)
{
    d_line = line;
}

void Port::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    //QPen pen;
    if(parentItem()->isSelected())
        setPen(QPen(Qt::blue));
    else
        setPen(QPen(Qt::black));

    //QLineF line;
    QRectF rect;
    int flag;
    if(d_direction == Port::IN){
        setLine(QLine(QPoint(0, 0), QPoint(3, 0)));
        rect = QRectF(3+1,-1.5,20,2.5);
        flag = Qt::AlignLeft;
    }
    else{
        setLine(QLine(QPoint(-3, 0), QPoint(0, 0)));
        rect = QRectF(-3-20,-1.5,19,2.5);
        flag = Qt::AlignRight;
    }
    painter->setPen(d_pen);
    //painter->drawPolygon(polygon);
    painter->drawLine(line());
    QFont font = QApplication::font();
    font.setPixelSize( rect.height() );
    painter->setFont( font );
    painter->drawText(rect, flag, name());
    painter->drawRect(d_anchor);
}

QRectF Port::boundingRect() const
{
    return QRectF(-3, -2, 6, 3);
}

void Port::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
//    //setPen(QPen(Qt::red));
//    //QGraphicsPolygonItem::hoverEnterEvent(event);
//    QGraphicsItem* item = scene()->itemAt(mapToScene(event->pos()));
//    item->setVisible(false);
////    Port* port = qgraphicsitem_cast<Port*>(item);
////    //qDebug() << item->data();
////    if (port){
//        qDebug() << "Hover Entered";
//        if(isSelected())
//            setPen(QPen(Qt::blue));
//        else
//            setPen(QPen(Qt::black));
////    }
    //d_anchor = QRectF(-0.5,-0.5,1,1);
}


