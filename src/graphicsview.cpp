
//Our includes
#include "graphicsview.h"
#include "math.h"
 
//Qt includes
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

/**
* Sets up the subclassed QGraphicsView
*/
GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
 
    //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //Set-up the scene
   // QGraphicsScene* Scene = new QGraphicsScene(this);
    //setScene(Scene);
 
    //Populate the scene
//    for(int x = 0; x < 1000; x = x + 25) {
//        for(int y = 0; y < 1000; y = y + 25) {
 
//            if(x % 100 == 0 && y % 100 == 0) {
//                Scene->addRect(x, y, 2, 2);
 
//                QString pointString;
//                QTextStream stream(&pointString);
//                stream << "(" << x << "," << y << ")";
//                QGraphicsTextItem* item = Scene->addText(pointString);
//                item->setPos(x, y);
//            } else {
//                Scene->addRect(x, y, 1, 1);
//            }
//        }
//    }
 
    //Set-up the view
 /*    setSceneRect(0, 0, 1000, 1000);
     SetCenter(QPointF(500.0, 500.0));*/ //A modified version of centerOn(), handles special cases
    //setCursor(Qt::OpenHandCursor);
    //setMouseTracking(true);
    setDragMode(QGraphicsView::RubberBandDrag);
}
 
/**
  * Sets the current centerpoint.  Also updates the scene's center point.
  * Unlike centerOn, which has no way of getting the floating point center
  * back, SetCenter() stores the center point.  It also handles the special
  * sidebar case.  This function will claim the centerPoint to sceneRec ie.
  * the centerPoint must be within the sceneRec.
  */
//Set the current centerpoint in the
void GraphicsView::SetCenter(const QPointF& centerPoint) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
 
    //Get the scene area
    QRectF sceneBounds = sceneRect();
 
    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;
 
    //The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);
 
    if(bounds.contains(centerPoint)) {
        //We are within the bounds
        CurrentCenterPoint = centerPoint;
    } else {
        //We need to clamp or use the center of the screen
        if(visibleArea.contains(sceneBounds)) {
            //Use the center of scene ie. we can see the whole scene
            CurrentCenterPoint = sceneBounds.center();
        } else {
 
            CurrentCenterPoint = centerPoint;
 
            //We need to clamp the center. The centerPoint is too large
            if(centerPoint.x() > bounds.x() + bounds.width()) {
                CurrentCenterPoint.setX(bounds.x() + bounds.width());
            } else if(centerPoint.x() < bounds.x()) {
                CurrentCenterPoint.setX(bounds.x());
            }
 
            if(centerPoint.y() > bounds.y() + bounds.height()) {
                CurrentCenterPoint.setY(bounds.y() + bounds.height());
            } else if(centerPoint.y() < bounds.y()) {
                CurrentCenterPoint.setY(bounds.y());
            }
 
        }
    }
 
    //Update the scrollbars
    centerOn(CurrentCenterPoint);
}
 
/**
  * Handles when the mouse button is pressed
  */
void GraphicsView::mousePressEvent(QMouseEvent* event) {
    if(event->modifiers() != Qt::ControlModifier)
    {
        QGraphicsView::mousePressEvent(event);
        return;
    }
    //For panning the view
    LastPanPoint = event->pos();
    setCursor(Qt::ClosedHandCursor);
}
 
/**
  * Handles when the mouse button is released
  */
void GraphicsView::mouseReleaseEvent(QMouseEvent* event) {
    if(event->modifiers() != Qt::ControlModifier)
    {
        QGraphicsView::mouseReleaseEvent(event);
        setCursor(Qt::ArrowCursor);
        return;
    }
    setCursor(Qt::OpenHandCursor);
    LastPanPoint = QPoint();
}
 
/**
*Handles the mouse move event
*/
void GraphicsView::mouseMoveEvent(QMouseEvent* event) {

    if(event->modifiers() != Qt::ControlModifier){
        QGraphicsView::mouseMoveEvent(event);
        return;
    }
    if(!LastPanPoint.isNull()) {
        //Get how much we panned
        QPointF delta = mapToScene(LastPanPoint) - mapToScene(event->pos());
        LastPanPoint = event->pos();
 
        //Update the center ie. do the pan
        SetCenter(GetCenter() + delta);
        //centerOn(GetCenter() + delta);

    }
}
 
/**
  * Zoom the view in and out.
  */
void GraphicsView::wheelEvent(QWheelEvent* event) {
 
    if(event->modifiers() != Qt::ControlModifier){
        QGraphicsView::wheelEvent(event);
        return;
    }
    //Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale(mapToScene(event->pos()));
 
    //Get the original screen centerpoint
    QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());
 
    //Scale the view ie. do the zoom
    double scaleFactor = 1.15; //How fast we zoom
    if(event->delta() > 0) {
        //Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        //Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
 
    //Get the position after scaling, in scene coords
    QPointF pointAfterScale(mapToScene(event->pos()));
 
    //Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;
 
    //Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    SetCenter(newCenter);
    //SetCenter(pointAfterScale);
    //centerOn(pointAfterScale);
}
 
/**
  * Need to update the center so there is no jolt in the
  * interaction after resizing the widget.
  */
void GraphicsView::resizeEvent(QResizeEvent* event) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    SetCenter(visibleArea.center());
 
    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}

void GraphicsView::drawBackground(QPainter *p, const QRectF &r) {
    p -> save();

    p -> setRenderHint(QPainter::Antialiasing, false);
    p -> setRenderHint(QPainter::TextAntialiasing, true);
    p -> setRenderHint(QPainter::SmoothPixmapTransform, false);

    p -> setPen(Qt::NoPen);
    p -> setBrush(Qt::white);
    p -> drawRect(r);

    QRectF drawable_area(rect().x(), rect().y(), rect().width(), rect().height());
    p -> setPen(Qt::black);
    p -> setBrush(Qt::NoBrush);
   // p -> drawRect(drawable_area);

    qreal zoom_factor = matrix().m11();
    bool draw_grid = true;
    bool draw_cross = false;
    int drawn_x_grid = 1;
    int drawn_y_grid = 1;

    if (zoom_factor < (0.5)) {
            // pas de grille du tout
            draw_grid = false;
    } else if (zoom_factor < 4.0) {
            // grille a 10 px
            drawn_x_grid *= 20;
            drawn_y_grid *= 20;
    } else if (zoom_factor < 6) {
            // grille a 2 px (avec croix)
            drawn_x_grid *= 4;
            drawn_y_grid *= 4;
            draw_cross = true;
    } else {
            // grille a 1 px (avec croix)
            draw_cross = true;
    }

    if (draw_grid) {
            // dessine les points de la grille
            p -> setPen(Qt::black);
            p -> setBrush(Qt::NoBrush);
            qreal limite_x = r.x() + r.width();
            qreal limite_y = r.y() + r.height();

            int g_x = (int)ceil(r.x());
            while (g_x % drawn_x_grid) ++ g_x;
            int g_y = (int)ceil(r.y());
            while (g_y % drawn_y_grid) ++ g_y;

            for (int gx = g_x ; gx < limite_x ; gx += drawn_x_grid) {
                    for (int gy = g_y ; gy < limite_y ; gy += drawn_y_grid) {
                            if (draw_cross) {
                                    if (!(gx % 10) && !(gy % 10)) {
                                            p -> drawLine(QLineF(gx - 0.25, gy, gx + 0.25, gy));
                                            p -> drawLine(QLineF(gx, gy - 0.25, gx, gy + 0.25));
                                    } else {
                                            p -> drawPoint(gx, gy);
                                    }
                            } else {
                                    p -> drawPoint(gx, gy);
                            }
                    }
            }
    }
    p -> restore();
}
