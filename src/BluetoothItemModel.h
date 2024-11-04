#ifndef BLUETOOTHITEMMODEL_H
#define BLUETOOTHITEMMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QBluetoothDeviceInfo>
#include "BluetoothExport.h"
namespace QtRockchip::Qt6Bluetooth{
class QTROCKCHIP_QT6BLUETOOTH_EXPORT BluetoothItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit BluetoothItemModel(QObject *parent = nullptr);
    void push(const QBluetoothDeviceInfo &info);
private:
    QVector<QBluetoothDeviceInfo> m_dataVector ;
    QVector<bool > m_lastCheckState;

signals:
    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
};
}
#endif // BLUETOOTHITEMMODEL_H
