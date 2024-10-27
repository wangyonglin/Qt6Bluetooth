#ifndef BLUETOOTHHANDLER_H
#define BLUETOOTHHANDLER_H

#include <QObject>
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
    void select(const QBluetoothDeviceInfo &info);
    void release();
private:
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QLowEnergyDescriptor notificationDescriptor;
    QBluetoothDeviceInfo localDevice;
    bool m_foundHeartRateService = false;
signals:
    void resolve(const QString & loginfo);
    void reject(const QString &logerr);
    void aliveChanged();
public slots:
    void discovered(const QBluetoothUuid &newService);
    void finished();
    void execution(QLowEnergyService::ServiceError error);
    void serviceStateChanged(QLowEnergyService::ServiceState newState);
    void updateHeartRateValue(const QLowEnergyCharacteristic &info,
                               const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &info,
                           const QByteArray &value);
};
}
#endif // BLUETOOTHHANDLER_H
