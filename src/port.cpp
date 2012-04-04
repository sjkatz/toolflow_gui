/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#include "port.h"
#include "component.h"
#include "graphicsscene.h"
#include <QPen>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


Port::Port(QString name, Direction direction, DataType type, int msb, int lsb, QString id, QGraphicsItem * parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene)
{
    setName(name);
    setDirection(direction);
    setDataType(type);
    setMsb(msb);
    setLsb(lsb);
    setId(id);
    d_pen = QPen(Qt::black);

    setAcceptHoverEvents(true);
    //setHandlesChildEvents(true);
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

QString Port::id()
{
    return d_id;
}

void Port::setId(QString id)
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

QString Port::directionString()
{
    if(d_direction == IN)
        return "IN";
    else if(d_direction == OUT)
        return "OUT";
    else if(d_direction == INOUT)
        return "INOUT";
    else
        return "";
}

QString Port::dataTypeString()
{
    if(d_type == STD_LOGIC)
        return "STD_LOGIC";
    else if(d_type == STD_LOGIC_VECTOR)
        return "STD_LOGIC_VECTOR";
    else
        return "";
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

QString Port::parentComponentId()
{
    if(parentItem())
        if(Component* comp = qgraphicsitem_cast<Component*>(parentItem()))
            return comp->id();

    return QString();
}

float Port::posFactor()
{
    return y()/parentItem()->boundingRect().height();
}

GraphicsScene* Port::graphicsScene()
{
    return qobject_cast<GraphicsScene*>(scene());
}

void Port::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    if(parentItem()->isSelected())
        setPen(QPen(Qt::red,0.4));
    else
        setPen(QPen(Qt::black,0.4));

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
    painter->drawLine(line());
    QFont font = QApplication::font();
    font.setPixelSize( rect.height() );
    font.setLetterSpacing(QFont::PercentageSpacing,120);
    painter->setFont( font );
    painter->drawText(rect, flag, name());
}

QRectF Port::boundingRect() const
{
    qreal extra = (d_pen.width());

    return QRectF(-3, -2, 6, 3);/*.normalized()
            .adjusted(-extra, -extra, extra, extra);*/
}

void Port::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    d_hover_indicator = new QGraphicsEllipseItem(this);
    d_hover_indicator->setRect(-1,-1,2,2);
    d_hover_indicator->setPen(QPen(Qt::blue));
    d_hover_indicator->setBrush(QBrush(Qt::blue));
}

void Port::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    delete d_hover_indicator;
    QGraphicsItem::hoverLeaveEvent(event);
}

void Port::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (GraphicsScene* scene = graphicsScene()) {
            scene->setWireStart(mouseEvent->scenePos());
            scene->setWireStop(mouseEvent-> scenePos());
            //scene->setWire(true);
            //setCursor(Qt::CrossCursor);
    }
}

void Port::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (GraphicsScene* scene = graphicsScene())
        scene->setWireStop(mouseEvent->scenePos());
}

void Port::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (GraphicsScene* scene = graphicsScene())
        scene->setWire();
}
