#include <QtGui/QApplication>
#include "CMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    CMainWindow window;
    window.show();
    
    return app.exec();
}
