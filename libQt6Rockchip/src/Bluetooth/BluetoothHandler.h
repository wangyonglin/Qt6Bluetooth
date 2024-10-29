#ifndef BLUETOOTHHANDLER_H
#define BLUETOOTHHANDLER_H

#include <QObject>
#include <QTimer>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyDescriptor>

namespace Qt6Rockchip::Bluetooth {

class BluetoothHandler : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothHandler(QObject *parent = nullptr);
    void create(const QBluetoothDeviceInfo &info);
    void release();
    void starting();
    void stop();
private:
    int milliseconds = 3000;
    QTimer *keep_alive;
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QLowEnergyDescriptor descriptor;
    QBluetoothDeviceInfo localdevice;
    QList<QBluetoothUuid> uuids;
    QBluetoothUuid useuuid;
    QLowEnergyCharacteristic characteristic;
    QList<QLowEnergyCharacteristic> characteristics;
signals:
    void resolve(const QString & loginfo);
    void reject(const QString &logerr);
    void transmit(const QByteArray &msg);
    void aliveChanged();
public slots:
    void connected();
    void disconnected();
    void serviceDiscovered(const QBluetoothUuid &newService);
    void discoveryFinished();
    void execution(QLowEnergyService::ServiceError error);
    void stateChanged(QLowEnergyService::ServiceState newState);
    void characteristicChanged(const QLowEnergyCharacteristic &info,
                               const QByteArray &value);
    void characteristicWritten(const QLowEnergyCharacteristic &info,
                               const QByteArray &value);
    void characteristicRead(const QLowEnergyCharacteristic &info,
                            const QByteArray &value);
};
}
#endif // BLUETOOTHHANDLER_H
