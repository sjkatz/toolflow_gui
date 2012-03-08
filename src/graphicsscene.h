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

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertLine, MoveItem };

    GraphicsScene(QObject *parent = 0);

public slots:
    void setMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent( QGraphicsSceneDragDropEvent * event );

private:
    Mode myMode;
    QPointF startPoint;
    QGraphicsLineItem *d_line;
    QPointF last_center_point;
};

#endif
