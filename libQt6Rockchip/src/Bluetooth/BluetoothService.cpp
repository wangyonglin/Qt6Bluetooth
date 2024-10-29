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
    connect(bluetoothFinder,&Qt6Rockchip::Bluetooth::BluetoothFinder::refresh,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::refresh);

    bluetoothHandler= new Qt6Rockchip::Bluetooth::BluetoothHandler(this);
    connect(bluetoothHandler,&Qt6Rockchip::Bluetooth::BluetoothHandler::resolve,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::resolve);
    connect(bluetoothHandler,&Qt6Rockchip::Bluetooth::BluetoothHandler::reject,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::reject);

    connect(bluetoothHandler,&Qt6Rockchip::Bluetooth::BluetoothHandler::transmit,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::receive);
}

Qt6Rockchip::Bluetooth::BluetoothService::~BluetoothService()
{
    bluetoothFinder->cleanup();
}

void Qt6Rockchip::Bluetooth::BluetoothService::start(const QString &name,int timeout)
{
    devicename=name;
    bluetoothFinder->search();
}


void Qt6Rockchip::Bluetooth::BluetoothService::refresh()
{

    for(QBluetoothDeviceInfo & device:bluetoothFinder->getBluetoothDeviceInfo()){
        qDebug() << "发现蓝牙设备:" << device.name() << device.address();
        if(device.name().contains(devicename)){
            bluetoothHandler->create(device);
            bluetoothHandler->starting();
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

void Qt6Rockchip::Bluetooth::BluetoothService::receive(const QByteArray &msg)
{
    emit transmit(msg);
}
