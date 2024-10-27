#ifndef BLUETOOTHSERVICE_H
#define BLUETOOTHSERVICE_H

#include <QWidget>
#include "BluetoothPair.h"
#include "BluetoothSearch.h"
#include "BluetoothConnect.h"

namespace Qt6Rockchip::Bluetooth{
class BluetoothService : public QWidget
{
    Q_OBJECT
public:
    explicit BluetoothService(QWidget *parent = nullptr);
    ~BluetoothService();
    void selecte(const QString & name);
private:

    QBluetoothDeviceInfo deviceinfo;
    Qt6Rockchip::Bluetooth::BluetoothPair *bluetoothPair;
    Qt6Rockchip::Bluetooth::BluetoothSearch *bluetoothSearch;
    Qt6Rockchip::Bluetooth::BluetoothConnect * bluetoothConnect;
public slots:
    void resolve(const QString & loginfo);
    void reject(const QString & logerror);
    void discovered(const QBluetoothDeviceInfo &deviceinfo);
    void discovering(QLowEnergyController*lowenergyController,const QBluetoothUuid & newService);
    void reader(const QLowEnergyCharacteristic &info,
                 const QByteArray &value);
signals:
};
}



#endif // BLUETOOTHSERVICE_H
