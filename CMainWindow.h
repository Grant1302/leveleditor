#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "CView.h"

class CMainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QGraphicsScene *Scene;
    CView *View;
    QMenu *FileMenu;
    QMenu *EditMenu;
    QAction *NewMapAct;
    QAction *OpenMapAct;
    QAction *SaveMapAct;
    QAction *SaveMapAsAct;
    QAction *ExitAct;
    QAction *OpenTilesetAct;
    QAction *ConfigAct;
    QToolBar *TilesToolBar;
    QLabel *BrushLabel;

private:
    QMap<QString, short> Config;
    QImage Tileset;
    short BrushTileID;
    short BrushTypeID;
    short StartX;
    short StartY;
    QString TilesetFile;
    QString MapFile;

public:
    explicit CMainWindow(QWidget *Parent = NULL);

private:
    void SetupMatrix();
    void CreateMap();
    void CreateActions();
    void CreateMenus();
    void LoadTileset(const QString &FileName);
    void ChangeTile(QMouseEvent *e);

private slots:
    void ShowConfig();
    void ActionTriggered(QAction *Action);
    void Map_MousePressed(QMouseEvent *e);
    void Map_MouseMoved(QMouseEvent *e);
    void NewMap();
    void OpenMap();
    void SaveMap();
    void SaveMapAs();
    void OpenTileset();
    void TileTypeNormal_Toggled(bool Checked);
    void TileTypeBlock_Toggled(bool Checked);
    void TileTypeBumpable_Toggled(bool Checked);
    void TileTypeBumpableCoin_Toggled(bool Checked);
    void TileTypeCoin_Toggled(bool Checked);
};

#endif
