#ifndef BLUETOOTHDEVICE_H
#define BLUETOOTHDEVICE_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QBluetoothDeviceInfo;
QT_END_NAMESPACE


namespace Qt6Rockchip::Bluetooth {
class DeviceInfo;
class BluetoothFinder : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothFinder(QObject *parent = nullptr);
    QList<QBluetoothDeviceInfo> getBluetoothDeviceInfo();
    void search();
    void cleanup();
public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void deviceExecution(QBluetoothDeviceDiscoveryAgent::Error error);
    void deviceFinished();
signals:
    void resolve(const QString & loginfo);
    void reject(const QString &logerr);
    void refresh();
private:
    QBluetoothDeviceDiscoveryAgent *agent;
    QList<QBluetoothDeviceInfo> listBluetoothDeviceInfo;

};
}
#endif // BLUETOOTHDEVICE_H
