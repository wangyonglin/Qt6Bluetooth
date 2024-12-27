#include <QApplication>
#include <QBluetoothPermission>
#include <QMessageBox>
#include <QVariant>
#include <QtRockchip/Qt6Bluetooth/BluetoothWidget>
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QtRockchip::Qt6Bluetooth::BluetoothWidget widget;
    widget.resize(800,600);
    widget.show();
    return app.exec();
}

