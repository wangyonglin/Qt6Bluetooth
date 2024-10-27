#include <QApplication>
#include <QBluetoothPermission>
#include <QMessageBox>
#include <QVariant>
#include <Qt6Rockchip/Bluetooth/BluetoothService>

int main(int argc, char *argv[])
{
// name: "53-F5-66-01-17-0F"
// name: "7D-78-0D-12-32-47"
// name: "FA-5B-3F-55-B4-7B"
// name: "C barcode scanner" address "DD:0D:30:9E:F9:B9"
    QApplication app(argc, argv);
\




    Qt6Rockchip::Bluetooth::BluetoothService service;
    service.resize(800,600);
    service.show();
    service.selecte("C barcode scanner");
   // service.selecte("KM-360A-6AAA_Ble");

    return app.exec();
}

