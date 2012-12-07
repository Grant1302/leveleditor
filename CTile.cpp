#include "CTile.h"

#include <QtGui>

CTile::CTile() :
    TileID(0),
    TypeID(TILE_TYPE_NONE),
    SwapTileID(0),
    SwapTypeID(TILE_TYPE_NONE)
{
}

CTile::CTile(short TileID, short TypeID) :
    TileID(TileID),
    TypeID(TypeID),
    SwapTileID(0),
    SwapTypeID(TILE_TYPE_NONE)
{
//    setFlags(ItemIsSelectable);
    setAcceptsHoverEvents(true);
}

QRectF CTile::boundingRect() const
{
    return QRectF(0, 0, TILE_SIZE, TILE_SIZE);
}

QPainterPath CTile::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, TILE_SIZE, TILE_SIZE);
    return path;
}

void CTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QColor color(240, 240, 240);
    QPen p = painter->pen();
    QBrush b = painter->brush();
    QRect tileRect(0, 0, TILE_SIZE, TILE_SIZE);
    QRect swapTileRect(TILE_SIZE / 4, TILE_SIZE / 4, TILE_SIZE - TILE_SIZE / 4, TILE_SIZE - TILE_SIZE / 4);

    painter->setPen(QColor::fromRgba(qRgba(200, 200, 200, 127)));
    painter->setBrush(color);
    painter->drawRect(0, 0, TILE_SIZE, TILE_SIZE);
    painter->setBrush(b);

    if (SwapTypeID > TILE_TYPE_NONE)
    {
        tileRect.setWidth(TILE_SIZE - TILE_SIZE / 4);
        tileRect.setHeight(TILE_SIZE - TILE_SIZE / 4);
    }

    DrawTile(painter, swapTileRect, SwapTileID, SwapTypeID);
    DrawTile(painter, tileRect, TileID, TypeID);

    painter->setPen(p);
}

void CTile::SetTileset(QImage *Tileset)
{
    this->Tileset = Tileset;
}

void CTile::DrawTile(QPainter *Painter, const QRect &Rect, short TileID, short TypeID)
{
    if (TypeID > TILE_TYPE_NONE)
    {
        int tilesInRow = Tileset->width() / TILE_SIZE;
        int row = TileID / tilesInRow;
        int col = TileID % tilesInRow;

        Painter->drawImage(Rect, Tileset->copy(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE));

        switch (TypeID)
        {
        case TILE_TYPE_BLOCK:
            Painter->setPen(QPen(QBrush(QColor::fromRgba(qRgba(255, 0, 0, 127))), 2));
            Painter->drawLine(Rect.left() + 1, Rect.top() + 1, Rect.left() + Rect.width() - 1, Rect.top() + Rect.height() - 1);
            Painter->drawLine(Rect.left() + Rect.width() - 1, Rect.top() + 1, Rect.left() + 1, Rect.top() + Rect.height() - 1);
            break;
        case TILE_TYPE_BUMPABLE:
            Painter->setPen(QPen(QBrush(QColor::fromRgba(qRgba(0, 255, 0, 127))), 2));
            Painter->drawRect(Rect);
            break;
        case TILE_TYPE_BUMPABLECOIN:
            Painter->setPen(QPen(QBrush(QColor::fromRgba(qRgba(255, 0, 255, 127))), 2));
            Painter->drawRect(Rect);
            break;
        case TILE_TYPE_COIN:
            Painter->setPen(QPen(QBrush(QColor::fromRgba(qRgba(0, 255, 255, 127))), 2));
            Painter->drawRect(Rect);
            break;
        }
    }
}
