#include "BluetoothSearch.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QTimer>

#if QT_CONFIG(permissions)
#include <QCoreApplication>
#include <QPermissions>
#endif

Qt6Rockchip::Bluetooth::BluetoothSearch::BluetoothSearch(QObject *parent)
    :BluetoothObject{parent}
{
    agent = new QBluetoothDeviceDiscoveryAgent(this);

    // 设置搜索设备超时 20s
    agent->setLowEnergyDiscoveryTimeout(6000);

    connect(agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &Qt6Rockchip::Bluetooth::BluetoothSearch::deviceDiscovered);
    connect(agent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &Qt6Rockchip::Bluetooth::BluetoothSearch::deviceExecution);
    connect(agent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &Qt6Rockchip::Bluetooth::BluetoothSearch::deviceFinished);

}

Qt6Rockchip::Bluetooth::BluetoothSearch::~BluetoothSearch()
{
    if(agent->isActive()){
        agent->stop();
    }
}


void Qt6Rockchip::Bluetooth::BluetoothSearch::search()
{
#if QT_CONFIG(permissions)
    //! [permissions]
    QBluetoothPermission permission{};
    permission.setCommunicationModes(QBluetoothPermission::Access);
    switch (qApp->checkPermission(permission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(permission, this, &Qt6Rockchip::Bluetooth::BluetoothSearch::search);
        return;
    case Qt::PermissionStatus::Denied:
        qDebug() << "Bluetooth permissions not granted!";
        return;
    case Qt::PermissionStatus::Granted:
        break; // proceed to search
    }
    //! [permissions]
#endif // QT_CONFIG(permissions)

    //  qDeleteAll(list_device);

    emit Resolved("bluetooth scan start");
    agent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

}


void Qt6Rockchip::Bluetooth::BluetoothSearch::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if( info.isValid()){
        if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
            emit Resolved(tr("bluetooth discovered deivce [%0 - %1]").arg(info.name()).arg(info.address().toString()));
            emit SearchSignal(info);
        }
    }

}

void Qt6Rockchip::Bluetooth::BluetoothSearch::deviceExecution(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError) {
        emit Rejected("bluetooth power off");
    } else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError) {
        emit Rejected("bluetooth input io error");
    } else {
        emit Rejected("bluetooth other error");
    }
}

void Qt6Rockchip::Bluetooth::BluetoothSearch::deviceFinished()
{
   emit Resolved("bluetooth scan end");
}
