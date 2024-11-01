#include "BluetoothService.h"

#include <QThread>

Qt6Rockchip::Bluetooth::BluetoothService::BluetoothService(QObject *parent)
    :BluetoothObject{parent}

{

}

Qt6Rockchip::Bluetooth::BluetoothService::~BluetoothService()
{

    if (controller) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }
}

void Qt6Rockchip::Bluetooth::BluetoothService::Init(const QBluetoothDeviceInfo &remoteDevice)
{

    // Disconnect and delete old connection
    emit Resolved("bluetooth init ...");

    if (service) {
        delete service;
        service = nullptr;
    }
    if (controller) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }

    if (remoteDevice.isValid()) {
        // Make connections
        emit Resolved(tr("bluetooth create ble [%0 - %1]").arg(remoteDevice.name()).arg(remoteDevice.address().toString()));
        controller = QLowEnergyController::createCentral(remoteDevice, this);
        if(controller){
            remote=remoteDevice;
            controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
            connect(controller, &QLowEnergyController::serviceDiscovered,
                    this, &Qt6Rockchip::Bluetooth::BluetoothService::serviceDiscovered);
            connect(controller, &QLowEnergyController::discoveryFinished,
                    this, &Qt6Rockchip::Bluetooth::BluetoothService::discoveryFinished);
            connect(controller, &QLowEnergyController::errorOccurred,
                    this,&Qt6Rockchip::Bluetooth::BluetoothService::controllerError);

            connect(controller, &QLowEnergyController::stateChanged,
                    this,&Qt6Rockchip::Bluetooth::BluetoothService::stateChangedController);

            connect(controller, &QLowEnergyController::connected,
                    this,&Qt6Rockchip::Bluetooth::BluetoothService::connected);
            connect(controller, &QLowEnergyController::disconnected,
                    this,&Qt6Rockchip::Bluetooth::BluetoothService::disconnected);
        }
        }

}
void Qt6Rockchip::Bluetooth::BluetoothService::connectToDevice(){
    if(controller) controller->connectToDevice();
}
void Qt6Rockchip::Bluetooth::BluetoothService::disconnectFromDevice()
{
    controller->disconnectFromDevice();
}

void Qt6Rockchip::Bluetooth::BluetoothService::stateChangedController(QLowEnergyController::ControllerState state)
{
    emit ControllerStateSignal(state);
    // if(state == QLowEnergyController::ControllerState::ClosingState){
    //     emit Resolved("bluetooth ClosingState state");
    // }else if(state == QLowEnergyController::ControllerState::ConnectedState){
    //     emit Resolved("bluetooth ConnectedState state");
    // }else if(state == QLowEnergyController::ControllerState::UnconnectedState){
    //     emit Resolved("bluetooth UnconnectedState state");
    // }


}

void Qt6Rockchip::Bluetooth::BluetoothService::controllerError(QLowEnergyController::Error error)
{
    //Q_UNUSED(error);
    emit Rejected(tr("bluetooth cannot connect to [%0 - %1]").arg(controller->remoteAddress().toString()).arg(error));
}



void Qt6Rockchip::Bluetooth::BluetoothService::connected()
{

    emit Resolved(tr("bluetooth  connected [%0]").arg(remote.name()));
    QThread::usleep(200);
    controller->discoverServices();

}

void Qt6Rockchip::Bluetooth::BluetoothService::disconnected()
{
    emit Resolved(tr("bluetooth  disconnected [%0]").arg(remote.name()));
}

void Qt6Rockchip::Bluetooth::BluetoothService::serviceDiscovered(const QBluetoothUuid &gatt)
{
    if(!gatt.isNull() && gatt==QBluetoothUuid( quint16(0xfff0))){

        if (service) {
            delete service;
            service = nullptr;
        }
        service = controller->createServiceObject(gatt, this);
        if (service) {
            connect(service, &QLowEnergyService::stateChanged,
                    this, &Qt6Rockchip::Bluetooth::BluetoothService::stateChanged);
            connect(service, &QLowEnergyService::characteristicChanged,
                    this, &Qt6Rockchip::Bluetooth::BluetoothService::characteristicChanged);
            connect(service, &QLowEnergyService::characteristicRead,
                    this, &Qt6Rockchip::Bluetooth::BluetoothService::characteristicRead);
            connect(service, &QLowEnergyService::characteristicWritten,
                    this, &Qt6Rockchip::Bluetooth::BluetoothService::characteristicWritten);
            connect(service, &QLowEnergyService::errorOccurred,
                    this, &Qt6Rockchip::Bluetooth::BluetoothService::serviceError);
            service->discoverDetails();
        }

    }

}

void Qt6Rockchip::Bluetooth::BluetoothService::discoveryFinished()
{
}

void Qt6Rockchip::Bluetooth::BluetoothService::serviceError(QLowEnergyService::ServiceError error)
{
    // qDebug() << tr("LowEnergy service error[%0]").arg(error);
}


void Qt6Rockchip::Bluetooth::BluetoothService::stateChanged(QLowEnergyService::ServiceState newState)
{
    QLowEnergyCharacteristic  batteryLevel;
    if(newState==QLowEnergyService::DiscoveringService){
        emit Resolved("Discovering services...");

    }else if(newState==QLowEnergyService::ServiceDiscovered){
        emit Resolved("Service discovered.");
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

void Qt6Rockchip::Bluetooth::BluetoothService::characteristicChanged(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if(info.isValid() && !value.isNull()){
        emit CharacteristicSignal(value);
    }

}

void Qt6Rockchip::Bluetooth::BluetoothService::characteristicWritten(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if (service)
    {
        if(characteristic.isValid()){
            service->writeCharacteristic(characteristic, value, QLowEnergyService::WriteWithResponse);
        }
    }
}


void Qt6Rockchip::Bluetooth::BluetoothService::characteristicRead(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if(info.isValid() && !value.isEmpty()){
        qDebug() << info.name()<< value.toStdString();
    }
}

