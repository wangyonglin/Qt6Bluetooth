#include "BluetoothService.h"

#include <QThread>

QtRockchip::Qt6Bluetooth::BluetoothService::BluetoothService(QObject *parent)
    : BluetoothObject(parent)

{
    time.setInterval(300);
    connect(&time,&QTimer::timeout,this,&QtRockchip::Qt6Bluetooth::BluetoothService::keepAlive);
}

QtRockchip::Qt6Bluetooth::BluetoothService::~BluetoothService()
{

    if (controller) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }
}

void QtRockchip::Qt6Bluetooth::BluetoothService::createCentral(const QBluetoothDeviceInfo &remote)
{
    if (remote.isValid()) {
        remoteBluetooth=remote;
        qInfo() << tr("bluetooth create ble [%0 - %1]").arg(remote.name()).arg(remote.address().toString());
        if(controller){ controller->disconnectFromDevice();}
         controller=QLowEnergyController::createCentral(remote,this);
        if(controller){
            controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
            connect(controller, &QLowEnergyController::serviceDiscovered,
                    this, &QtRockchip::Qt6Bluetooth::BluetoothService::createServiceObject,Qt::DirectConnection);
            // connect(controller, &QLowEnergyController::discoveryFinished,
            //         this, &QtRockchip::Qt6Bluetooth::BluetoothService::discoveryFinished);
            connect(controller, &QLowEnergyController::errorOccurred,
                    this,&QtRockchip::Qt6Bluetooth::BluetoothService::controllerError,Qt::DirectConnection);

            connect(controller, &QLowEnergyController::stateChanged,
                    this,&QtRockchip::Qt6Bluetooth::BluetoothService::stateChangedController,Qt::DirectConnection);

            connect(controller, &QLowEnergyController::connected,
                    this,&QtRockchip::Qt6Bluetooth::BluetoothService::connected,Qt::DirectConnection);
            connect(controller, &QLowEnergyController::disconnected,
                    this,&QtRockchip::Qt6Bluetooth::BluetoothService::disconnected,Qt::DirectConnection);
            controller->connectToDevice();
        }
    }

}



void QtRockchip::Qt6Bluetooth::BluetoothService::stateChangedController(QLowEnergyController::ControllerState state)
{
    if(state == QLowEnergyController::ControllerState::ClosingState){
        qDebug() << "bluetooth ClosingState state";
        time.start();
    }else if(state == QLowEnergyController::ControllerState::ConnectedState){
        qDebug() <<"bluetooth ConnectedState state";
        time.stop();
    }else if(state == QLowEnergyController::ControllerState::UnconnectedState){
        qDebug() <<"bluetooth UnconnectedState state";
        time.start();
    }
}

void QtRockchip::Qt6Bluetooth::BluetoothService::controllerError(QLowEnergyController::Error error)
{
    //Q_UNUSED(error);
    qDebug() << tr("bluetooth cannot connect to [%0 - %1]").arg(controller->remoteAddress().toString()).arg(error);
    if(remoteBluetooth.isValid() && error == QLowEnergyController::Error::UnknownError){
        time.start();
    }

}



void QtRockchip::Qt6Bluetooth::BluetoothService::connected()
{

    qDebug() << "bluetooth  connected" << controller->remoteName();
    controller->discoverServices();

}

void QtRockchip::Qt6Bluetooth::BluetoothService::disconnected()
{
    qDebug() << "bluetooth  disconnected" << controller->remoteName();
    time.start(100);

}

void QtRockchip::Qt6Bluetooth::BluetoothService::createServiceObject(const QBluetoothUuid &gatt)
{
    qInfo() << "createServiceObject" << gatt.toString();
    if(!gatt.isNull() && gatt==QBluetoothUuid( quint16(0xfff0))){

        if (service) {
            delete service;
            service = nullptr;
        }
        service = controller->createServiceObject(gatt, this);
        if (service) {
            connect(service, &QLowEnergyService::stateChanged,
                    this, &QtRockchip::Qt6Bluetooth::BluetoothService::stateChanged);
            connect(service, &QLowEnergyService::characteristicChanged,
                    this, &QtRockchip::Qt6Bluetooth::BluetoothService::characteristicChanged,Qt::DirectConnection);
            connect(service, &QLowEnergyService::characteristicRead,
                    this, &QtRockchip::Qt6Bluetooth::BluetoothService::characteristicRead);
            connect(service, &QLowEnergyService::characteristicWritten,
                    this, &QtRockchip::Qt6Bluetooth::BluetoothService::characteristicWritten);
            // connect(service, &QLowEnergyService::errorOccurred,
            //         this, &QtRockchip::Qt6Bluetooth::BluetoothService::serviceError);
            service->discoverDetails();
        }

    }

}




void QtRockchip::Qt6Bluetooth::BluetoothService::stateChanged(QLowEnergyService::ServiceState newState)
{
    QLowEnergyCharacteristic  batteryLevel;
    if(newState==QLowEnergyService::DiscoveringService){
        qInfo() << "Discovering services...";

    }else if(newState==QLowEnergyService::ServiceDiscovered){
        qInfo() << "Service discovered.";
        if(service){
            characteristics = service->characteristics();
            for(int i=0; i<characteristics.size(); i++){
                batteryLevel = characteristics.at(i);
                if(batteryLevel.isValid()){

                    descriptor = batteryLevel.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
                    if(descriptor.isValid()){
                        service->writeDescriptor(descriptor,QByteArray::fromHex("0100"));
                    }
                    if (batteryLevel.properties() & QLowEnergyCharacteristic::WriteNoResponse || batteryLevel.properties() & QLowEnergyCharacteristic::Write)
                    {

                        emit Resolved(tr("%0 WriteNoResponse").arg(service->serviceName()));
                        characteristic = batteryLevel;
                    }
                    if(batteryLevel.properties() & QLowEnergyCharacteristic::Read) {
                        emit Resolved(tr("%0 Read").arg(service->serviceName()));

                        characteristic = batteryLevel;
                    }
                    if(batteryLevel.properties() & QLowEnergyCharacteristic::Notify) {
                        emit Resolved(tr("%0 Notify").arg(service->serviceName()));

                        characteristic = batteryLevel;
                    }
                }

            }
        }

    }else if(newState==QLowEnergyService::DiscoveryRequired){
        emit Rejected("DiscoveryRequired was renamed to RemoteService.");
    }

}

void QtRockchip::Qt6Bluetooth::BluetoothService::characteristicChanged(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if(info.isValid() && !value.isNull()){
        emit CharacteristicSignal(value);
    }

}

void QtRockchip::Qt6Bluetooth::BluetoothService::characteristicWritten(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if (service)
    {
        if(characteristic.isValid()){
            service->writeCharacteristic(characteristic, value, QLowEnergyService::WriteWithResponse);
        }
    }
}


void QtRockchip::Qt6Bluetooth::BluetoothService::characteristicRead(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if(info.isValid() && !value.isEmpty()){
        qDebug() << info.name()<< value.toStdString();
    }
}

void QtRockchip::Qt6Bluetooth::BluetoothService::keepAlive(){

    if(controller->state()!=QLowEnergyController::ControllerState::ConnectedState){
        controller->connectToDevice();
    }
}
