#include "BluetoothSearch.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QTimer>

Qt6Rockchip::Bluetooth::BluetoothSearch::BluetoothSearch(QObject *parent)
    :QObject{parent}
{
    agent = new QBluetoothDeviceDiscoveryAgent(this);

    // 设置搜索设备超时 20s
    agent->setLowEnergyDiscoveryTimeout(6000);

    connect(agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &Qt6Rockchip::Bluetooth::BluetoothSearch::deviceDiscovered);
    connect(agent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &Qt6Rockchip::Bluetooth::BluetoothSearch::execution);
    connect(agent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &Qt6Rockchip::Bluetooth::BluetoothSearch::finished);
    connect(&agentTimeout, &QTimer::timeout, [this](){
        emit reject(QString::fromLocal8Bit("蓝牙控制器连接设备超时！"));
        stop();
    });

}
void Qt6Rockchip::Bluetooth::BluetoothSearch::start()
{
    agent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

}

void Qt6Rockchip::Bluetooth::BluetoothSearch::stop()
{
    if(agent->isActive()){
        agent->stop();
    }

}

void Qt6Rockchip::Bluetooth::BluetoothSearch::select(int index)
{
    if((devices.size()>0)&& (index >=0)){
        QBluetoothDeviceInfo deviceinfo= devices.at(index);
        if(deviceinfo.isValid()){
             emit discovered(deviceinfo);
        }
    }
}
void Qt6Rockchip::Bluetooth::BluetoothSearch::scan(const QString &devicename)
{
    if((devices.size()>0)){
        for(QBluetoothDeviceInfo deviceinfo :devices){
            if(deviceinfo.name().contains(devicename)){
                if(deviceinfo.isValid()){
                    emit discovered(deviceinfo);
                }
            }
        }
    }
}
void Qt6Rockchip::Bluetooth::BluetoothSearch::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        emit resolve(QString::fromLocal8Bit("发现一个蓝牙设备:%1--地址:%2").arg(info.name()).arg(info.address().toString()));
        devices.append(info);
        // 添加对发现的BLE设备名称过滤，只保留需要名称的设备
        if (info.name().contains(devicename) && !devicename.isEmpty())
        {
            // 保存设备信息
           emit discovered(info);
        }
    }

}

void Qt6Rockchip::Bluetooth::BluetoothSearch::execution(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError) {
        emit reject("蓝牙适配器已关闭");
    } else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError) {
        emit reject("蓝牙适配器IO错误");
    } else {
        emit reject("蓝牙适配器其它错误");
    }
}

void Qt6Rockchip::Bluetooth::BluetoothSearch::finished()
{
    emit sender(this->devices);
    emit resolve("蓝牙适配器扫描结束");
}
