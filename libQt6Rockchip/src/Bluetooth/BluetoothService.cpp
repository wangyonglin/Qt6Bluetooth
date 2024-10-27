#include "BluetoothService.h"
#include <QDebug>

Qt6Rockchip::Bluetooth::BluetoothService::BluetoothService(QWidget *parent)
    : QWidget{parent}
{

    bluetoothFinder= new Qt6Rockchip::Bluetooth::BluetoothFinder(this);
    connect(bluetoothFinder,&Qt6Rockchip::Bluetooth::BluetoothFinder::resolve,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::resolve);
    connect(bluetoothFinder,&Qt6Rockchip::Bluetooth::BluetoothFinder::reject,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::reject);
    connect(bluetoothFinder,&Qt6Rockchip::Bluetooth::BluetoothFinder::devices,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::devices);

    bluetoothHandler= new Qt6Rockchip::Bluetooth::BluetoothHandler(this);
    connect(bluetoothHandler,&Qt6Rockchip::Bluetooth::BluetoothHandler::resolve,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::resolve);
    connect(bluetoothHandler,&Qt6Rockchip::Bluetooth::BluetoothHandler::reject,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::reject);






}

Qt6Rockchip::Bluetooth::BluetoothService::~BluetoothService()
{

    bluetoothFinder->cleanup();
}

void Qt6Rockchip::Bluetooth::BluetoothService::selecte(const QString &name)
{
    devicename=name;
    bluetoothFinder->search();
}

void Qt6Rockchip::Bluetooth::BluetoothService::devices(QList<QBluetoothDeviceInfo > devices)
{

    for(QBluetoothDeviceInfo & device:devices){ 
        if(device.name().contains(devicename)){
            bluetoothHandler->select(device);
        }
    }
}

void Qt6Rockchip::Bluetooth::BluetoothService::resolve(const QString &loginfo)
{
    qInfo()  << loginfo;
}

void Qt6Rockchip::Bluetooth::BluetoothService::reject(const QString &logerror)
{
    qDebug()  << logerror;
}

void Qt6Rockchip::Bluetooth::BluetoothService::discovered(const QBluetoothDeviceInfo &device)
{
    if(device.isValid()){
       qDebug() << "发现蓝牙设备:" << device.name() << device.address();
    }


}
