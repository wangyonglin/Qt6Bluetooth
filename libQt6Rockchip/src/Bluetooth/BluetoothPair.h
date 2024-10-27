#ifndef BLUETOOTHPAIR_H
#define BLUETOOTHPAIR_H

#include <QObject>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QThread>
#include <QTimer>
#include "BluetoothConnect.h"
namespace Qt6Rockchip::Bluetooth {
    class BluetoothPair : public QObject
    {
        Q_OBJECT
    public:
        explicit BluetoothPair(QObject *parent = nullptr);
        QLowEnergyController* pairing(const QBluetoothDeviceInfo &deviceinfo);
        void cleanup();
    public slots:
        void connected();
        void disconnected();
        void finished();
        void discovered(const QBluetoothUuid &newService);
        void stateChanged(QLowEnergyController::ControllerState state);
        void execution(QLowEnergyController::Error newError);
    signals:
        void resolve(const QString & loginfo);
        void reject(const QString & reject);
        void sender(const QList<QBluetoothUuid> & newServices);
        void paired(QLowEnergyController*lowenergyController, const QBluetoothUuid & newService);
    private:

        QBluetoothDeviceInfo  bluetoothDeviceInfo;
        QLowEnergyController *lowenergyController;
        QTimer LECTimer;
        QList<QBluetoothUuid> uuids;
    };
}


#endif // BLUETOOTHPAIR_H
