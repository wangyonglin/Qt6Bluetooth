#include "BluetoothHandler.h"



Qt6Rockchip::Bluetooth::BluetoothHandler::BluetoothHandler(QObject *parent)
    :QObject{parent},
    milliseconds(3000)
{
    keep_alive= new QTimer(this);
    keep_alive->setInterval(milliseconds);
    connect(keep_alive, &QTimer::timeout,
            this,&Qt6Rockchip::Bluetooth::BluetoothHandler::starting);
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::create(const QBluetoothDeviceInfo &localdevice)
{

    // Disconnect and delete old connection
    if (controller) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }

    if (localdevice.isValid()) {
        // Make connections
        controller = QLowEnergyController::createCentral(localdevice, this);
        controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
        connect(controller, &QLowEnergyController::serviceDiscovered,
                this, &Qt6Rockchip::Bluetooth::BluetoothHandler::serviceDiscovered);
        connect(controller, &QLowEnergyController::discoveryFinished,
                this, &Qt6Rockchip::Bluetooth::BluetoothHandler::discoveryFinished);
        connect(controller, &QLowEnergyController::errorOccurred,
                this,&Qt6Rockchip::Bluetooth::BluetoothHandler::controllerError);
        connect(controller, &QLowEnergyController::connected,
                this,&Qt6Rockchip::Bluetooth::BluetoothHandler::connected);
        connect(controller, &QLowEnergyController::disconnected,
                this,&Qt6Rockchip::Bluetooth::BluetoothHandler::disconnected);


    }
}
void Qt6Rockchip::Bluetooth::BluetoothHandler::starting(){
    if(controller) controller->connectToDevice();
}
void Qt6Rockchip::Bluetooth::BluetoothHandler::stop()
{
    controller->disconnectFromDevice();
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::controllerError(QLowEnergyController::Error newError)
{
    Q_UNUSED(newError);
    qDebug() << "Cannot connect to remote bluetooth device.";
}



void Qt6Rockchip::Bluetooth::BluetoothHandler::connected()
{
    qInfo() << "Controller connected. bluetooth services...";
    keep_alive->stop();
    controller->discoverServices();
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::disconnected()
{
    qInfo() << "LowEnergy controller bluetooth disconnected";
    keep_alive->start();
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::serviceDiscovered(const QBluetoothUuid &gatt)
{
    qDebug() <<tr("DeviceInformation service discovered. Waiting for service scan to be done... uuid %0").arg(gatt.toString());
    if(!gatt.isNull() && gatt==QBluetoothUuid( quint16(0xfff0))){

        if (service) {
            delete service;
            service = nullptr;
        }
        service = controller->createServiceObject(gatt, this);
        if (service) {
            connect(service, &QLowEnergyService::stateChanged,
                    this, &Qt6Rockchip::Bluetooth::BluetoothHandler::stateChanged);
            connect(service, &QLowEnergyService::characteristicChanged,
                    this, &Qt6Rockchip::Bluetooth::BluetoothHandler::characteristicChanged);
            connect(service, &QLowEnergyService::characteristicRead,
                    this, &Qt6Rockchip::Bluetooth::BluetoothHandler::characteristicRead);
            connect(service, &QLowEnergyService::characteristicWritten,
                    this, &Qt6Rockchip::Bluetooth::BluetoothHandler::characteristicWritten);
            connect(service, &QLowEnergyService::errorOccurred,
                    this, &Qt6Rockchip::Bluetooth::BluetoothHandler::serviceError);
            service->discoverDetails();
        } else {
            qDebug() <<"Heart Rate Service not found.";
        }

    }

}

void Qt6Rockchip::Bluetooth::BluetoothHandler::discoveryFinished()
{
    qInfo() << "Service scan done.";


}

void Qt6Rockchip::Bluetooth::BluetoothHandler::serviceError(QLowEnergyService::ServiceError error)
{
   qDebug() << tr("LowEnergy service error[%0]").arg(error);
}


void Qt6Rockchip::Bluetooth::BluetoothHandler::stateChanged(QLowEnergyService::ServiceState newState)
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
                        qInfo() <<"QLowEnergyCharacteristic::WriteNoResponse";
                        characteristic = batteryLevel;
                    }
                    if(batteryLevel.properties() & QLowEnergyCharacteristic::Read) {
                        qInfo() << "QLowEnergyCharacteristic::Read";
                        characteristic = batteryLevel;
                    }
                    if(batteryLevel.properties() & QLowEnergyCharacteristic::Notify) {
                        qInfo() << "QLowEnergyCharacteristic::Notify";
                        characteristic = batteryLevel;
                    }
                }

            }
        }

    }else{
        qDebug() << "nothing for now";
    }
    emit aliveChanged();
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::characteristicChanged(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if(info.isValid() && !value.isNull()){
        emit transmit(value);
    }

}

void Qt6Rockchip::Bluetooth::BluetoothHandler::characteristicWritten(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if (service)
    {
        if(characteristic.isValid()){
            service->writeCharacteristic(characteristic, value, QLowEnergyService::WriteWithResponse);
        }
    }
}


void Qt6Rockchip::Bluetooth::BluetoothHandler::characteristicRead(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if(info.isValid() && !value.isEmpty()){
         qDebug() << info.name()<< value.toStdString();
    }
}

