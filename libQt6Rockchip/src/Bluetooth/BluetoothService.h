#ifndef BLUETOOTHSERVICE_H
#define BLUETOOTHSERVICE_H

#include <QWidget>
#include "BluetoothFinder.h"
#include "BluetoothHandler.h"
#include "BluetoothExport.h"
namespace Qt6Rockchip::Bluetooth{
class QT6ROCKCHIP_BLUETOOTH_EXPORT BluetoothService : public QWidget
{
    Q_OBJECT
public:
    explicit BluetoothService(QWidget *parent = nullptr);
    ~BluetoothService();
    void start(const QString & name,int timeout=3);

private:
    QString devicename;
    Qt6Rockchip::Bluetooth::BluetoothFinder *bluetoothFinder;
    Qt6Rockchip::Bluetooth::BluetoothHandler * bluetoothHandler;
public slots:
    void resolve(const QString & loginfo);
    void reject(const QString & logerror);
    void receive(const QByteArray & msg);
    void refresh();

signals:
    void transmit(const QByteArray & msg);
};
}



#endif // BLUETOOTHSERVICE_H
