#include <QApplication>
#include <QBluetoothPermission>
#include <QMessageBox>
#include <QVariant>
#include "BluetoothWidget.h"
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    BluetoothWidget widget;
    widget.resize(800,600);
    widget.show();
    return app.exec();
}

