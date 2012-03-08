/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   02/03/2012
****************************************************************************/

#ifndef PORT_H
#define PORT_H

#include <QString>
#include <QGraphicsItem>
#include <QPen>


class Port : public QGraphicsItem
{
public:
    enum Direction {
             IN = 0,
             OUT = 1,
             INOUT = 2
         };
    enum DataType {
             STD_LOGIC = 0,
             STD_LOGIC_VECTOR = 1
         };
    enum { Type = /*UserType + */267 };

    Port(QString name = QString(), Direction direction = IN, DataType type = STD_LOGIC, int msb = 0, int lsb = 0, int id = 0, QGraphicsItem * parent = 0, QGraphicsScene *scene = 0);
    Port(Port& ref);
    Port& operator=(Port& ref);

    int type() const
        { return Type; }

    int id();
    void setId(int id);
    QString name();
    void setName(QString name);
    Direction direction();
    void setDirection(Direction direction);
    DataType dataType();
    void setDataType(DataType type);
    int msb();
    void setMsb(int msb);
    int lsb();
    void setLsb(int lsb);
    int width();
    void setWidth(int width);
    QPen pen();
    void setPen(QPen pen);
    QLineF line();
    void setLine(QLineF line);

private:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    QRectF boundingRect() const;
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );

private:
    int d_id;
    QString d_name;
    Direction d_direction;
    DataType d_type;
    int d_msb;
    int d_lsb;
    int d_width;
    QPen d_pen;
    QLineF d_line;
    QRectF d_anchor;
};

#endif // PORT_H
