#include "BluetoothManager.h"
#include <QDebug>

Qt6Rockchip::Bluetooth::BluetoothManager::BluetoothManager(QObject *parent)
    : BluetoothObject{parent},
    keep_alive(new QTimer(this))
{
    connect(keep_alive, &QTimer::timeout,
            this,&Qt6Rockchip::Bluetooth::BluetoothManager::Keepalive);

    bluetoothSearch = new Qt6Rockchip::Bluetooth::BluetoothSearch(this);
    connect(bluetoothSearch,&Qt6Rockchip::Bluetooth::BluetoothSearch::SearchSignal,
            this,&Qt6Rockchip::Bluetooth::BluetoothManager::SearchSignal);

    connect(bluetoothSearch,&Qt6Rockchip::Bluetooth::BluetoothSearch::Resolved,
            this,&Qt6Rockchip::Bluetooth::BluetoothManager::ErrorSignal);

    connect(bluetoothSearch,&Qt6Rockchip::Bluetooth::BluetoothSearch::Rejected,
            this,&Qt6Rockchip::Bluetooth::BluetoothManager::ErrorSignal);

    bluetoothService= new Qt6Rockchip::Bluetooth::BluetoothService(this);

    connect(bluetoothService,&Qt6Rockchip::Bluetooth::BluetoothService::CharacteristicSignal,
            this,&Qt6Rockchip::Bluetooth::BluetoothManager::CharacteristichSignal);

    connect(bluetoothService,&Qt6Rockchip::Bluetooth::BluetoothService::ControllerStateSignal,
            this,&Qt6Rockchip::Bluetooth::BluetoothManager::ControllerStateSignal);

    connect(bluetoothService,&Qt6Rockchip::Bluetooth::BluetoothService::Resolved,
            this,&Qt6Rockchip::Bluetooth::BluetoothManager::ErrorSignal);

    connect(bluetoothService,&Qt6Rockchip::Bluetooth::BluetoothService::Rejected,
            this,&Qt6Rockchip::Bluetooth::BluetoothManager::ErrorSignal);

}

Qt6Rockchip::Bluetooth::BluetoothManager::~BluetoothManager()
{

}

void Qt6Rockchip::Bluetooth::BluetoothManager::searchBluetooth()
{
    bluetoothSearch->search();
}
void Qt6Rockchip::Bluetooth::BluetoothManager::disconnectBluetooth(){
    bluetoothService->disconnectFromDevice();
}
void Qt6Rockchip::Bluetooth::BluetoothManager::Keepalive()
{
    bluetoothService->connectToDevice();
}



void Qt6Rockchip::Bluetooth::BluetoothManager::connectBluetooth(const QBluetoothDeviceInfo &selectdevcie)
{
    if(selectdevcie.isValid()){

        bluetoothService->Init(selectdevcie);
        keep_alive->start(1000);
    }

}



