#ifndef BLUETOOTHWIDGET_H
#define BLUETOOTHWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QtRockchip/Qt6Bluetooth/BluetoothManager>
#include <QtRockchip/Qt6Bluetooth/BluetoothItemModel>
class BluetoothWidget : public QWidget
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

#endif // BLUETOOTHWIDGET_H
