#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include "BluetoothObject.h"
#include "BluetoothSearch.h"
#include "BluetoothService.h"
#include "BluetoothExport.h"
namespace QtRockchip::Qt6Bluetooth{
class QTROCKCHIP_QT6BLUETOOTH_EXPORT BluetoothManager : public BluetoothObject
{
    Q_OBJECT
public:
    explicit BluetoothManager(QObject *parent = nullptr);
    ~BluetoothManager();
    void searchBluetooth();
    void connectBluetooth(const QBluetoothDeviceInfo &selectdevcie);

private:
    QLowEnergyController::ControllerState state;
    QtRockchip::Qt6Bluetooth::BluetoothSearch *bluetoothSearch;
    QtRockchip::Qt6Bluetooth::BluetoothService * bluetoothService;

signals:
    void ControllerStateSignal(const QLowEnergyController::ControllerState &state);
    void CharacteristichSignal(const QByteArray & data);
    void SearchSignal(const QBluetoothDeviceInfo &info);
    void ErrorSignal(const QString & data);
};
}



#endif // BLUETOOTHMANAGER_H
