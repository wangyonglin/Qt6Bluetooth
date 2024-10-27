#ifndef BLUETOOTHSERVICE_H
#define BLUETOOTHSERVICE_H

#include <QWidget>
#include "BluetoothFinder.h"
#include "BluetoothHandler.h"

namespace Qt6Rockchip::Bluetooth{
class BluetoothService : public QWidget
{
    Q_OBJECT
public:
    explicit BluetoothService(QWidget *parent = nullptr);
    ~BluetoothService();
    void selecte(const QString & name);
private:
    QString devicename;
    Qt6Rockchip::Bluetooth::BluetoothFinder *bluetoothFinder;
    Qt6Rockchip::Bluetooth::BluetoothHandler * bluetoothHandler;
public slots:
    void devices(QList<QBluetoothDeviceInfo> devices);
    void resolve(const QString & loginfo);
    void reject(const QString & logerror);
    void discovered(const QBluetoothDeviceInfo &deviceinfo);

signals:
};
}



#endif // BLUETOOTHSERVICE_H
