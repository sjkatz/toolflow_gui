/****************************************************************************
    AUTHOR: Shaun Katz
            University of Cape Town (UCT)
            Department of Electrical Engineering
            Software Defined Radio Group (SDRG)
    DATE:   06/03/2012
****************************************************************************/

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include "component.h"
#include "wire.h"

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertLine, MoveItem };

    GraphicsScene(QObject *parent = 0);
    QList<Component*> components();
    void addComponent(Component* component);
    void removeComponent(Component* component);
    Component* findComponent(QString id);

    QList<Wire*> wires();
    void addWire(Wire* wire);
    void removeWire(Wire* wire);

    void setWireStart(QPointF point);
    void setWireStop(QPointF point);
    void setWire();

signals:
    void itemMoved(QGraphicsItem* moved_item, const QPointF &old_pos);
    void addGraphicsItem(QGraphicsItem* item);

public slots:
    void setMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent( QGraphicsSceneDragDropEvent * event );

private:
    Mode myMode;
    QPointF startPoint;
    QGraphicsLineItem *d_line;
    QPointF last_center_point;
    QList<Component*> d_components;
    QList<Wire*> d_wires;
    int d_wire_count;
    int d_component_count;
    QGraphicsItem* d_moving_item;
    QPointF d_old_item_pos;

};

#endif
