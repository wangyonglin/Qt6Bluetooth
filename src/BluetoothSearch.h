#ifndef BLUETOOTHDEVICE_H
#define BLUETOOTHDEVICE_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QTimer>
#include "BluetoothObject.h"
QT_BEGIN_NAMESPACE
class QBluetoothDeviceInfo;
QT_END_NAMESPACE


namespace QtRockchip::Qt6Bluetooth {
class DeviceInfo;
class BluetoothSearch : public BluetoothObject
{
    Q_OBJECT
public:
    explicit BluetoothSearch(QObject *parent = nullptr);
    ~BluetoothSearch();
    void search();
public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void deviceExecution(QBluetoothDeviceDiscoveryAgent::Error error);
    void deviceFinished();
signals:
    void SearchSignal(const QBluetoothDeviceInfo &info);
private:
    QBluetoothDeviceDiscoveryAgent *agent;


};
}
#endif // BLUETOOTHDEVICE_H
