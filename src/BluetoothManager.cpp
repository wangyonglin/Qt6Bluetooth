#include "BluetoothManager.h"
#include <QDebug>

QtRockchip::Qt6Bluetooth::BluetoothManager::BluetoothManager(QObject *parent)
    : BluetoothObject{parent}
{


    bluetoothSearch = new QtRockchip::Qt6Bluetooth::BluetoothSearch(this);
    connect(bluetoothSearch,&QtRockchip::Qt6Bluetooth::BluetoothSearch::SearchSignal,
            this,&QtRockchip::Qt6Bluetooth::BluetoothManager::SearchSignal);

    connect(bluetoothSearch,&QtRockchip::Qt6Bluetooth::BluetoothSearch::Resolved,
            this,&QtRockchip::Qt6Bluetooth::BluetoothManager::ErrorSignal);

    connect(bluetoothSearch,&QtRockchip::Qt6Bluetooth::BluetoothSearch::Rejected,
            this,&QtRockchip::Qt6Bluetooth::BluetoothManager::ErrorSignal);

    bluetoothService= new QtRockchip::Qt6Bluetooth::BluetoothService(this);

    connect(bluetoothService,&QtRockchip::Qt6Bluetooth::BluetoothService::CharacteristicSignal,
            this,&QtRockchip::Qt6Bluetooth::BluetoothManager::CharacteristichSignal);

    connect(bluetoothService,&QtRockchip::Qt6Bluetooth::BluetoothService::ControllerStateSignal,
            this,&QtRockchip::Qt6Bluetooth::BluetoothManager::ControllerStateSignal);

    connect(bluetoothService,&QtRockchip::Qt6Bluetooth::BluetoothService::Resolved,
            this,&QtRockchip::Qt6Bluetooth::BluetoothManager::ErrorSignal);

    connect(bluetoothService,&QtRockchip::Qt6Bluetooth::BluetoothService::Rejected,
            this,&QtRockchip::Qt6Bluetooth::BluetoothManager::ErrorSignal);

}

QtRockchip::Qt6Bluetooth::BluetoothManager::~BluetoothManager()
{

}

void QtRockchip::Qt6Bluetooth::BluetoothManager::searchBluetooth()
{
    bluetoothSearch->search();
}

void QtRockchip::Qt6Bluetooth::BluetoothManager::connectBluetooth(const QBluetoothDeviceInfo &selectdevcie)
{
    if(selectdevcie.isValid()){
        bluetoothService->createCentral(selectdevcie);
    }

}



