#include "BluetoothItemModel.h"
#include "qbluetoothaddress.h"
#include <QBluetoothDeviceInfo>

Qt6Rockchip::Bluetooth::BluetoothItemModel::BluetoothItemModel(QObject *parent)
    : QAbstractItemModel{parent}
{


}

QModelIndex Qt6Rockchip::Bluetooth::BluetoothItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row > m_dataVector.size() - 1 || row < 0)
        return  QModelIndex();
    if (column >4 || column < 0)
        return QModelIndex();
    return createIndex(row, column);
}

QModelIndex Qt6Rockchip::Bluetooth::BluetoothItemModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int Qt6Rockchip::Bluetooth::BluetoothItemModel::rowCount(const QModelIndex &parent) const
{
    return m_dataVector.size();
}

int Qt6Rockchip::Bluetooth::BluetoothItemModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant Qt6Rockchip::Bluetooth::BluetoothItemModel::data(const QModelIndex &index, int role) const
{
    QVariant rerult;
    if (!index.isValid())//索引是否有效
        return rerult;
    if (index.row() > m_dataVector.size() - 1 || index.row() < 0 || index.column() < 0 || index.column() > 4)///索引是否在指定范围
        return rerult;

    if(role == Qt::DisplayRole){
        QBluetoothDeviceInfo  info = m_dataVector.at(index.row());
        if (index.column() == 0)
        {
            rerult.setValue(info.name());
            return rerult;
        }
        if (index.column() == 1)
        {
            rerult.setValue(info.address().toString());
            return rerult;
        }

    }if(role==Qt::UserRole){

        QBluetoothDeviceInfo  info = m_dataVector.at(index.row());
        rerult.setValue(info);
        return rerult;
    }

    return rerult;

}
void Qt6Rockchip::Bluetooth::BluetoothItemModel:: push(const QBluetoothDeviceInfo &info)
{
    if(info.isValid()){
        auto it = std::find_if(m_dataVector.begin(), m_dataVector.end(),
                               [info](const QBluetoothDeviceInfo & dev) {
                                   return info.address() == dev.address();
                               });
        if (it == m_dataVector.end()) {
            beginInsertRows(QModelIndex(), m_dataVector.size(), m_dataVector.size());
            m_dataVector.push_back(info);
            endInsertRows();
        }
    }
}


