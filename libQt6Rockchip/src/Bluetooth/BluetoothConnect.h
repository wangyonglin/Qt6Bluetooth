#ifndef BLUETOOTHCONNECT_H
#define BLUETOOTHCONNECT_H

#include <QObject>
#include <QTimer>
#include <QThread>

#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

namespace Qt6Rockchip::Bluetooth {
    class BluetoothConnect : public QObject
    {
        Q_OBJECT
    public:
        explicit BluetoothConnect(QObject *parent = nullptr);
        ~BluetoothConnect();

        void create(const QBluetoothUuid &service);
        void cleanup();
        void init(QLowEnergyController*lowenergyController);
    public slots:

        void stateChanged(QLowEnergyService::ServiceState newState);
        void characteristicChanged(const QLowEnergyCharacteristic &info, const QByteArray &value);
        void characteristicRead(const QLowEnergyCharacteristic &info,
                                const QByteArray &value);
        void characteristicWritten(const QLowEnergyCharacteristic &info,
                                   const QByteArray &value);
        void execution(QLowEnergyService::ServiceError error);
    signals:
        void resolve(const QString & loginfo);
        void reject(const QString &logerr);
        void reader(const QLowEnergyCharacteristic &info,
                     const QByteArray &value);
    private:
        QLowEnergyController*lowenergyController=nullptr;
        QList<QBluetoothUuid> uuids;
        QTimer LECTimer;
        QLowEnergyService *LEService;
        QList<QLowEnergyCharacteristic>  listLECharacteristic;
        QLowEnergyDescriptor  LEDescriptor;
        QLowEnergyCharacteristic LECharacteristic;
        void link();
    };

}


#endif // BLUETOOTHCONNECT_H
