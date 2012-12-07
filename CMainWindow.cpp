#include "CMainWindow.h"
#include "CTile.h"
#include "CConfigDialog.h"

CMainWindow::CMainWindow(QWidget *Parent) :
    QMainWindow(Parent),
    BrushTileID(-1),
    BrushTypeID(TILE_TYPE_NORMAL)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    Scene = new QGraphicsScene;
    View = new CView;
    View->setScene(Scene);
    QObject::connect(View, SIGNAL(mousePressed(QMouseEvent *)), this, SLOT(Map_MousePressed(QMouseEvent *)));
    QObject::connect(View, SIGNAL(mouseMoved(QMouseEvent *)), this, SLOT(Map_MouseMoved(QMouseEvent *)));

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::StyledPanel);

    BrushLabel = new QLabel;
    BrushLabel->setMinimumWidth(TILE_SIZE);

    QRadioButton *ttNormalRadio = new QRadioButton;
    ttNormalRadio->setText("Normal");
    ttNormalRadio->setChecked(true);
    QObject::connect(ttNormalRadio, SIGNAL(toggled(bool)), this, SLOT(TileTypeNormal_Toggled(bool)));

    QRadioButton *ttBlockRadio = new QRadioButton;
    ttBlockRadio->setText("Block");
    QObject::connect(ttBlockRadio, SIGNAL(toggled(bool)), this, SLOT(TileTypeBlock_Toggled(bool)));

    QRadioButton *ttBumpableRadio = new QRadioButton;
    ttBumpableRadio->setText("Bumpable");
    QObject::connect(ttBumpableRadio, SIGNAL(toggled(bool)), this, SLOT(TileTypeBumpable_Toggled(bool)));

    QRadioButton *ttBumpableCoinRadio = new QRadioButton;
    ttBumpableCoinRadio->setText("Bumpable Coin");
    QObject::connect(ttBumpableCoinRadio, SIGNAL(toggled(bool)), this, SLOT(TileTypeBumpableCoin_Toggled(bool)));

    QRadioButton *ttCoinRadio = new QRadioButton;
    ttCoinRadio->setText("Coin");
    QObject::connect(ttCoinRadio, SIGNAL(toggled(bool)), this, SLOT(TileTypeCoin_Toggled(bool)));

    QVBoxLayout *toolsLayout = new QVBoxLayout;
    toolsLayout->addWidget(BrushLabel, 0, Qt::AlignCenter);
    toolsLayout->addWidget(ttNormalRadio);
    toolsLayout->addWidget(ttBlockRadio);
    toolsLayout->addWidget(ttBumpableRadio);
    toolsLayout->addWidget(ttBumpableCoinRadio);
    toolsLayout->addWidget(ttCoinRadio);
    toolsLayout->addStretch();
    frame->setLayout(toolsLayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(View);
    layout->addWidget(frame);
    widget->setLayout(layout);

    CreateActions();
    CreateMenus();
    TilesToolBar = addToolBar("");
    LoadTileset("level1.png");

    setWindowTitle(tr("Level Editor"));

    Config["width"] = 200;
    Config["height"] = 15;
    CreateMap();
}

void CMainWindow::CreateMap()
{
    Scene->clear();

    for (int i = 0; i < Config["width"]; i++)
    {
        for (int j = 0; j < Config["height"]; j++)
        {
            CTile *tile = new CTile(0, TILE_TYPE_NONE);
            tile->setPos(QPointF(i * TILE_SIZE, j * TILE_SIZE));
            tile->SetTileset(&Tileset);
            Scene->addItem(tile);
        }
    }
}

void CMainWindow::CreateActions()
{
    NewMapAct = new QAction(tr("&New Map"), this);
    NewMapAct->setShortcuts(QKeySequence::New);
    NewMapAct->setStatusTip(tr("Create a new file"));
    connect(NewMapAct, SIGNAL(triggered()), this, SLOT(NewMap()));

    OpenMapAct = new QAction(tr("&Open Map"), this);
    OpenMapAct->setShortcuts(QKeySequence::Open);
    OpenMapAct->setStatusTip(tr("Open an existing file"));
    connect(OpenMapAct, SIGNAL(triggered()), this, SLOT(OpenMap()));

    SaveMapAct = new QAction(tr("&Save Map"), this);
    SaveMapAct->setShortcuts(QKeySequence::Save);
    SaveMapAct->setStatusTip(tr("Save the document to disk"));
    connect(SaveMapAct, SIGNAL(triggered()), this, SLOT(SaveMap()));

    SaveMapAsAct = new QAction(tr("Save Map &As..."), this);
    SaveMapAsAct->setShortcuts(QKeySequence::SaveAs);
    SaveMapAsAct->setStatusTip(tr("Save the document to disk"));
    connect(SaveMapAsAct, SIGNAL(triggered()), this, SLOT(SaveMapAs()));

    ExitAct = new QAction(tr("E&xit"), this);
    ExitAct->setShortcuts(QKeySequence::Quit);
    ExitAct->setStatusTip(tr("Exit the application"));
    connect(ExitAct, SIGNAL(triggered()), this, SLOT(close()));

    OpenTilesetAct = new QAction(tr("&Open Tileset"), this);
    OpenTilesetAct->setStatusTip(tr("Open an existing tileset file"));
    connect(OpenTilesetAct, SIGNAL(triggered()), this, SLOT(OpenTileset()));

    ConfigAct = new QAction(tr("&Map Configuration"), this);
    ConfigAct->setStatusTip(tr("Open map configuration dialog"));
    connect(ConfigAct, SIGNAL(triggered()), this, SLOT(ShowConfig()));
}

void CMainWindow::CreateMenus()
{
    FileMenu = menuBar()->addMenu(tr("&File"));
    FileMenu->addAction(NewMapAct);
    FileMenu->addAction(OpenMapAct);
    FileMenu->addSeparator();
    FileMenu->addAction(SaveMapAct);
    FileMenu->addAction(SaveMapAsAct);
    FileMenu->addSeparator();
    FileMenu->addAction(ExitAct);
    EditMenu = menuBar()->addMenu(tr("&Edit"));
    EditMenu->addAction(OpenTilesetAct);
    EditMenu->addAction(ConfigAct);
}

void CMainWindow::LoadTileset(const QString &FileName)
{
    TilesetFile = FileName;
#ifdef WIN32
    Tileset.load(QString("data\\") + TilesetFile);
#else
    Tileset.load(QString("data/") + TilesetFile);
#endif
    TilesToolBar->clear();

    int tilesInRow = Tileset.width() / TILE_SIZE;

    for (int i = 0; i < Tileset.height() / TILE_SIZE; i++)
    {
        for (int j = 0; j < Tileset.width() / TILE_SIZE; j++)
        {
            QAction *action = TilesToolBar->addAction(QPixmap::fromImage(Tileset.copy(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE)), "");
            action->setCheckable(true);
            action->setData(i * tilesInRow + j);
        }
    }
    TilesToolBar->setIconSize(QSize(TILE_SIZE, TILE_SIZE));
    QObject::connect(TilesToolBar, SIGNAL(actionTriggered(QAction *)), this, SLOT(ActionTriggered(QAction *)));
}

void CMainWindow::ShowConfig()
{
    CConfigDialog *dialog = new CConfigDialog(Config, this);

    if (dialog->exec())
    {
        Config = dialog->Result();
        CreateMap();
        View->ResetView(Config);
    }
}

void CMainWindow::ActionTriggered(QAction *Action)
{
    foreach (QAction *action, TilesToolBar->actions())
        action->setChecked(false);

    Action->setChecked(true);

    BrushTileID = Action->data().toInt();
    int tilesInRow = Tileset.width() / TILE_SIZE;
    int row = BrushTileID / tilesInRow;
    int col = BrushTileID % tilesInRow;
    BrushLabel->setPixmap(QPixmap::fromImage(Tileset.copy(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE)));
}

void CMainWindow::Map_MousePressed(QMouseEvent *e)
{
    ChangeTile(e);
}

void CMainWindow::Map_MouseMoved(QMouseEvent *e)
{
    ChangeTile(e);
}

void CMainWindow::ChangeTile(QMouseEvent *e)
{
    QPointF pt = View->mapToScene(e->pos());
    CTile *tile = (CTile *)Scene->itemAt(pt);

    if (tile == NULL)
        return;

    if (e->modifiers() == Qt::ShiftModifier)
    {
        if (e->buttons() & Qt::LeftButton)
        {
            if (BrushTileID != -1)
            {
                tile->SwapTileID = BrushTileID;
                tile->SwapTypeID = BrushTypeID;
            }
        }
        else if (e->buttons() & Qt::RightButton)
        {
            tile->SwapTileID = 0;
            tile->SwapTypeID = TILE_TYPE_NONE;
        }
    }
    else if (e->modifiers() == Qt::NoModifier)
    {
        if (e->buttons() & Qt::LeftButton)
        {
            if (BrushTileID != -1)
            {
                tile->TileID = BrushTileID;
                tile->TypeID = BrushTypeID;
            }
        }
        else if (e->buttons() & Qt::RightButton)
        {
            tile->TileID = 0;
            tile->TypeID = TILE_TYPE_NONE;
        }
    }

    Scene->invalidate();
}

void CMainWindow::NewMap()
{
    CreateMap();
    MapFile = "";
}

void CMainWindow::OpenMap()
{
    QFileDialog dialog(this, "Open Map", QDir::currentPath(), "Map File (*.map)");

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec())
    {
        MapFile = dialog.selectedFiles()[0];
        FILE *file = fopen(MapFile.toAscii().data(), "rb");

        if (file)
        {
            short len;
            char tilesetFile[255];
            short width, height;

            Scene->clear();

            fread(&len, sizeof(len), 1, file);
            fread(tilesetFile, len, 1, file);
            LoadTileset(tilesetFile);

            fread(&width, sizeof(width), 1, file);
            fread(&height, sizeof(height), 1, file);
            Config["width"] = width;
            Config["height"] = height;

            for (int Y = 0; Y < height; Y++)
            {
                for (int X = 0; X < width; X++)
                {
                    CTile *tile = new CTile;

                    fread(&tile->TileID, sizeof(tile->TileID), 1, file);
                    fread(&tile->TypeID, sizeof(tile->TypeID), 1, file);
                    fread(&tile->SwapTileID, sizeof(tile->SwapTileID), 1, file);
                    fread(&tile->SwapTypeID, sizeof(tile->SwapTypeID), 1, file);
                    tile->setPos(QPointF(X * TILE_SIZE, Y * TILE_SIZE));
                    tile->SetTileset(&Tileset);
                    Scene->addItem(tile);
                }
            }
            fread(&StartX, sizeof(StartX), 1, file);
            fread(&StartY, sizeof(StartY), 1, file);

            fclose(file);
            View->ResetView(Config);
            resize(size().width() + 1, size().height());
            resize(size().width() - 1, size().height());
        }
    }
}

void CMainWindow::SaveMap()
{
    if (MapFile.isEmpty())
    {
        SaveMapAs();

        if (MapFile.isEmpty())
            return;
    }

    FILE *file = fopen(MapFile.toAscii().data(), "wb");

    if (file)
    {
        short width = Config["width"];
        short height = Config["height"];
        int count = Scene->items().count();
        short len = TilesetFile.length() + 1;

        fwrite(&len, sizeof(len), 1, file);
        fwrite(TilesetFile.toAscii().data(), len, 1, file);
        fwrite(&width, sizeof(width), 1, file);
        fwrite(&height, sizeof(height), 1, file);

        for (int i = count - 1; i >= 0; i--)
        {
            CTile *tile = (CTile *)Scene->items()[i];

            fwrite(&tile->TileID, sizeof(tile->TileID), 1, file);
            fwrite(&tile->TypeID, sizeof(tile->TypeID), 1, file);
            fwrite(&tile->SwapTileID, sizeof(tile->SwapTileID), 1, file);
            fwrite(&tile->SwapTypeID, sizeof(tile->SwapTypeID), 1, file);
        }
        fwrite(&StartX, sizeof(StartX), 1, file);
        fwrite(&StartY, sizeof(StartY), 1, file);

        fclose(file);
    }
}

void CMainWindow::SaveMapAs()
{
    QFileDialog dialog(this, "Save Map As...", QDir::currentPath(), "Map File (*.map)");

    dialog.setDefaultSuffix("map");
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    if (!MapFile.isEmpty())
    {
        QFileInfo info(MapFile);
        dialog.selectFile(info.fileName());
    }

    if (dialog.exec())
    {
        MapFile = dialog.selectedFiles()[0];
        SaveMap();
    }
}

void CMainWindow::OpenTileset()
{
    QFileDialog dialog(this, "Open Tileset", QDir::currentPath(), "PNG File (*.png)");

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec())
#ifdef WIN32
        LoadTileset(dialog.selectedFiles()[0].section("\\", -1, -1));
#else
        LoadTileset(dialog.selectedFiles()[0].section("/", -1, -1));
#endif
}

void CMainWindow::TileTypeNormal_Toggled(bool Checked)
{
    if (Checked)
        BrushTypeID = TILE_TYPE_NORMAL;
}

void CMainWindow::TileTypeBlock_Toggled(bool Checked)
{
    if (Checked)
        BrushTypeID = TILE_TYPE_BLOCK;
}

void CMainWindow::TileTypeBumpable_Toggled(bool Checked)
{
    if (Checked)
        BrushTypeID = TILE_TYPE_BUMPABLE;
}

void CMainWindow::TileTypeBumpableCoin_Toggled(bool Checked)
{
    if (Checked)
        BrushTypeID = TILE_TYPE_BUMPABLECOIN;
}

void CMainWindow::TileTypeCoin_Toggled(bool Checked)
{
    if (Checked)
        BrushTypeID = TILE_TYPE_COIN;
}
