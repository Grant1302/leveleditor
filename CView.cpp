#include "CView.h"

#include <QtGui>
#include <qmath.h>
#include "CTile.h"

CView::CView(QWidget *Parent)
    : QGraphicsView(Parent)
{
    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::RubberBandDrag);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void CView::ResetView(QMap<QString, short> &Config)
{
    int width = Config["width"] * TILE_SIZE;
    int height = Config["height"] * TILE_SIZE;

    resize(width, height);
    ensureVisible(QRectF(0, 0, 0, height));
}

void CView::mousePressEvent(QMouseEvent *e)
{
    mousePressed(e);
}

void CView::mouseMoveEvent(QMouseEvent *e)
{
    mouseMoved(e);
}
