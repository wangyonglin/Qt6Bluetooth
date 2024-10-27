#ifndef BLUETOOTHDEVICE_H
#define BLUETOOTHDEVICE_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QTimer>

namespace Qt6Rockchip::Bluetooth {

class BluetoothSearch : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothSearch(QObject *parent = nullptr);
    void start();
    void stop();
    void select(int index);
    void scan(const QString &devicename);
public slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void execution(QBluetoothDeviceDiscoveryAgent::Error error);
    void finished();
signals:
    void resolve(const QString & loginfo);
    void reject(const QString &logerr);
    void discovered(const QBluetoothDeviceInfo &deviceinfo);
    void sender(const QList<QBluetoothDeviceInfo> &devices);
private:
    QString devicename;
    QBluetoothDeviceDiscoveryAgent *agent;
    QList<QBluetoothDeviceInfo> devices;
    QTimer agentTimeout;
};
}
#endif // BLUETOOTHDEVICE_H
