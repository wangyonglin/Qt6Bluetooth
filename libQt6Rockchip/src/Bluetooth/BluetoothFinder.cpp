#include "BluetoothFinder.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QTimer>

#if QT_CONFIG(permissions)
#include <QCoreApplication>
#include <QPermissions>
#endif

Qt6Rockchip::Bluetooth::BluetoothFinder::BluetoothFinder(QObject *parent)
    :QObject{parent}
{
    agent = new QBluetoothDeviceDiscoveryAgent(this);

    // 设置搜索设备超时 20s
    agent->setLowEnergyDiscoveryTimeout(6000);

    connect(agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &Qt6Rockchip::Bluetooth::BluetoothFinder::deviceDiscovered);
    connect(agent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &Qt6Rockchip::Bluetooth::BluetoothFinder::deviceExecution);
    connect(agent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &Qt6Rockchip::Bluetooth::BluetoothFinder::deviceFinished);
    // connect(&agentTimeout, &QTimer::timeout, [this](){
    //     emit reject(QString::fromLocal8Bit("蓝牙控制器连接设备超时！"));
    //     stop();
    // });

}
void Qt6Rockchip::Bluetooth::BluetoothFinder::search()
{
#if QT_CONFIG(permissions)
    //! [permissions]
    QBluetoothPermission permission{};
    permission.setCommunicationModes(QBluetoothPermission::Access);
    switch (qApp->checkPermission(permission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(permission, this, &Qt6Rockchip::Bluetooth::BluetoothFinder::search);
        return;
    case Qt::PermissionStatus::Denied:
        emit reject(tr("Bluetooth permissions not granted!"));
        return;
    case Qt::PermissionStatus::Granted:
        break; // proceed to search
    }
    //! [permissions]
#endif // QT_CONFIG(permissions)

  //  qDeleteAll(list_device);
    list_device.clear();

    agent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

}

void Qt6Rockchip::Bluetooth::BluetoothFinder::cleanup()
{
    if(agent->isActive()){
        agent->stop();
    }

}

void Qt6Rockchip::Bluetooth::BluetoothFinder::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    if(device.isValid()){
        emit discovered(device);
        // If device is LowEnergy-device, add it to the list
        if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {

            auto it = std::find_if(list_device.begin(), list_device.end(),
                                   [device](QBluetoothDeviceInfo dev) {
                                       return device.address() == dev.address();
                                   });
            if (it == list_device.end()) {
                list_device.append(device);
            } else {
            }
            emit changed();
        }
    }


}

void Qt6Rockchip::Bluetooth::BluetoothFinder::deviceExecution(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError) {
        emit reject("蓝牙适配器已关闭");
    } else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError) {
        emit reject("蓝牙适配器IO错误");
    } else {
        emit reject("蓝牙适配器其它错误");
    }
}

void Qt6Rockchip::Bluetooth::BluetoothFinder::deviceFinished()
{
    emit resolve("蓝牙适配器扫描结束");
    emit devices(list_device);
}
