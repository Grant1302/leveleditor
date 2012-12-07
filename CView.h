#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>

QT_FORWARD_DECLARE_CLASS(QLabel)

class CView : public QGraphicsView
{
    Q_OBJECT

public:
    CView(QWidget *Parent = NULL);
    void ResetView(QMap<QString, short> &Config);

signals:
    void mousePressed(QMouseEvent *e);
    void mouseMoved(QMouseEvent *e);

public slots:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
};

#endif
