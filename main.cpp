#include "magrathea.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Magrathea mainWindow;
    mainWindow.show();

    return app.exec();
}
