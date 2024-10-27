#include "BluetoothService.h"
#include <QDebug>

Qt6Rockchip::Bluetooth::BluetoothService::BluetoothService(QWidget *parent)
    : QWidget{parent}
{

    bluetoothSearch= new Qt6Rockchip::Bluetooth::BluetoothSearch(this);
    connect(bluetoothSearch,&Qt6Rockchip::Bluetooth::BluetoothSearch::resolve,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::resolve);
    connect(bluetoothSearch,&Qt6Rockchip::Bluetooth::BluetoothSearch::reject,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::reject);
    connect(bluetoothSearch,&Qt6Rockchip::Bluetooth::BluetoothSearch::discovered,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::discovered);


    bluetoothPair= new Qt6Rockchip::Bluetooth::BluetoothPair(this);
    connect(bluetoothPair,&Qt6Rockchip::Bluetooth::BluetoothPair::resolve,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::resolve);
    connect(bluetoothPair,&Qt6Rockchip::Bluetooth::BluetoothPair::reject,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::reject);
    connect(bluetoothPair,&Qt6Rockchip::Bluetooth::BluetoothPair::paired,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::discovering);

    bluetoothConnect=new Qt6Rockchip::Bluetooth::BluetoothConnect(this);
    connect(bluetoothConnect,&Qt6Rockchip::Bluetooth::BluetoothConnect::resolve,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::resolve);
    connect(bluetoothConnect,&Qt6Rockchip::Bluetooth::BluetoothConnect::reject,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::reject);

    connect(bluetoothConnect,&Qt6Rockchip::Bluetooth::BluetoothConnect::reader,
            this,&Qt6Rockchip::Bluetooth::BluetoothService::reader);




}

Qt6Rockchip::Bluetooth::BluetoothService::~BluetoothService()
{
    bluetoothConnect->cleanup();
    bluetoothPair->cleanup();
    bluetoothSearch->stop();
}

void Qt6Rockchip::Bluetooth::BluetoothService::selecte(const QString &name)
{
    bluetoothSearch->start();
    bluetoothSearch->scan(name);
}

void Qt6Rockchip::Bluetooth::BluetoothService::resolve(const QString &loginfo)
{
    qInfo()  << loginfo;
}

void Qt6Rockchip::Bluetooth::BluetoothService::reject(const QString &logerror)
{
    qDebug()  << logerror;
}

void Qt6Rockchip::Bluetooth::BluetoothService::discovered(const QBluetoothDeviceInfo &deviceinfo)
{
    qDebug() << "已选蓝牙设备:" << deviceinfo.name();
   bluetoothPair->pairing(deviceinfo);

}



void Qt6Rockchip::Bluetooth::BluetoothService::discovering(QLowEnergyController*lowenergyController,const QBluetoothUuid &service)
{

    qDebug() << "已收蓝牙UUID:" << service.toString();
    bluetoothConnect->init(lowenergyController);
    bluetoothConnect->create(service);
}

void Qt6Rockchip::Bluetooth::BluetoothService::reader(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
     qDebug() << "设备名称:" << info.name()<< "接收数据:" << value.toStdString();
}
