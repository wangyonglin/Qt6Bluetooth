#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include "BluetoothObject.h"
#include "BluetoothSearch.h"
#include "BluetoothService.h"
#include "BluetoothExport.h"
namespace Qt6Rockchip::Bluetooth{
class QT6ROCKCHIP_BLUETOOTH_EXPORT BluetoothManager : public BluetoothObject
{
    Q_OBJECT
public:
    explicit BluetoothManager(QObject *parent = nullptr);
    ~BluetoothManager();
    void searchBluetooth();
    void connectBluetooth(const QBluetoothDeviceInfo &selectdevcie);
    void disconnectBluetooth();
private:
    QTimer *keep_alive;
    QLowEnergyController::ControllerState state;
    Qt6Rockchip::Bluetooth::BluetoothSearch *bluetoothSearch;
    Qt6Rockchip::Bluetooth::BluetoothService * bluetoothService;
public slots:
    void Keepalive();
signals:
    void ControllerStateSignal(const QLowEnergyController::ControllerState &state);
    void CharacteristichSignal(const QByteArray & data);
    void SearchSignal(const QBluetoothDeviceInfo &info);
    void ErrorSignal(const QString & data);
};
}



#endif // BLUETOOTHMANAGER_H
