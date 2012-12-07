#ifndef CHIP_H
#define CHIP_H

#include <QtGui/QColor>
#include <QtGui/QGraphicsItem>

#define TILE_SIZE 16

enum
{
    TILE_TYPE_NONE = 0,
    TILE_TYPE_NORMAL,
    TILE_TYPE_BLOCK,
    TILE_TYPE_BUMPABLE,
    TILE_TYPE_BUMPABLECOIN,
    TILE_TYPE_COIN
};

class CTile : public QGraphicsItem
{
public:
    short TileID;
    short TypeID;
    short SwapTileID;
    short SwapTypeID;

private:
    QImage *Tileset;

public:
    explicit CTile();
    CTile(short TileID, short TypeID);

public:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    void SetTileset(QImage *Tileset);

private:
    void DrawTile(QPainter *Painter, const QRect &Rect, short TileID, short TypeID);
};

#endif
