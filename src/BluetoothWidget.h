#ifndef BLUETOOTHWIDGET_H
#define BLUETOOTHWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include "BluetoothManager.h"
#include "BluetoothItemModel.h"
namespace QtRockchip::Qt6Bluetooth{
class QTROCKCHIP_QT6BLUETOOTH_EXPORT BluetoothWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BluetoothWidget(QWidget *parent = nullptr);
private:
    QStringList bluetoothName;
    QTextEdit *editlogger;
    QtRockchip::Qt6Bluetooth::BluetoothItemModel* bluetoothModel;
    QtRockchip::Qt6Bluetooth::BluetoothManager *bluetoothManager;
public Q_SLOTS:
    void logger(const QString & log);
signals:
};
}

#endif // BLUETOOTHWIDGET_H
