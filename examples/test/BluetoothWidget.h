#ifndef BLUETOOTHWIDGET_H
#define BLUETOOTHWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <Qt6Rockchip/Bluetooth/BluetoothManager>
#include <Qt6Rockchip/Bluetooth/BluetoothItemModel>
class BluetoothWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BluetoothWidget(QWidget *parent = nullptr);
private:
    QStringList bluetoothName;
    QTextEdit *editlogger;
    Qt6Rockchip::Bluetooth::BluetoothItemModel* bluetoothModel;
    Qt6Rockchip::Bluetooth::BluetoothManager *bluetoothManager;
public Q_SLOTS:
    void logger(const QString & log);
signals:
};

#endif // BLUETOOTHWIDGET_H
