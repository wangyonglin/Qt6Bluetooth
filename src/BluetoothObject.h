#ifndef BLUETOOTHOBJECT_H
#define BLUETOOTHOBJECT_H

#include <QObject>
namespace QtRockchip::Qt6Bluetooth {
class BluetoothObject : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothObject(QObject *parent = nullptr);
public Q_SLOTS:

signals:
    void Rejected(const QString &bytes);
    void Resolved(const QString &bytes);
    // executioner
};
}


#endif // BLUETOOTHOBJECT_H
