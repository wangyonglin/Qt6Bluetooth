#include "BluetoothHandler.h"



Qt6Rockchip::Bluetooth::BluetoothHandler::BluetoothHandler(QObject *parent)
    :QObject{parent}
{

}

void Qt6Rockchip::Bluetooth::BluetoothHandler::select(const QBluetoothDeviceInfo &localDevice)
{

    // Disconnect and delete old connection
    if (controller) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }
    // Create new controller and connect it if device available
    if (localDevice.isValid()) {
        // Make connections
        //! [Connect-Signals-1]
        controller = QLowEnergyController::createCentral(localDevice, this);
        //! [Connect-Signals-1]
        //controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
        //! [Connect-Signals-2]
        connect(controller, &QLowEnergyController::serviceDiscovered,
                this, &Qt6Rockchip::Bluetooth::BluetoothHandler::discovered);
        connect(controller, &QLowEnergyController::discoveryFinished,
                this, &Qt6Rockchip::Bluetooth::BluetoothHandler::finished);

        connect(controller, &QLowEnergyController::errorOccurred, this,
                [this](QLowEnergyController::Error error) {
                    Q_UNUSED(error);
                    emit reject("Cannot connect to remote device.");

                });
        connect(controller, &QLowEnergyController::connected, this, [this]() {
            emit resolve("Controller connected. Search services...");
            controller->discoverServices();
        });
        connect(controller, &QLowEnergyController::disconnected, this, [this]() {
            emit reject("LowEnergy controller disconnected");

        });

        // Connect
        controller->connectToDevice();
        //! [Connect-Signals-2]
        emit resolve(tr("创建[%0]控制器").arg(localDevice.name()));
    }
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::release()
{
    controller->disconnectFromDevice();
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::discovered(const QBluetoothUuid &gatt)
{

    qDebug() <<"UUID"<< gatt.toString();
   if (gatt == QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::HeartRate)) {
       emit resolve("Heart Rate service discovered. Waiting for service scan to be done...");
        m_foundHeartRateService = true;
   }
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::finished()
{
    emit resolve("Service scan done.");


    // Delete old service if available
    if (service) {
        delete service;
        service = nullptr;
    }

    //! [Filter HeartRate service 2]
    // If heartRateService found, create new service
    if (m_foundHeartRateService)
        service = controller->createServiceObject(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::HeartRate), this);

    if (service) {
        connect(service, &QLowEnergyService::stateChanged,
                this, &Qt6Rockchip::Bluetooth::BluetoothHandler::serviceStateChanged);
        connect(service, &QLowEnergyService::characteristicChanged,
                this, &Qt6Rockchip::Bluetooth::BluetoothHandler::updateHeartRateValue);
        connect(service, &QLowEnergyService::descriptorWritten,
                this, &Qt6Rockchip::Bluetooth::BluetoothHandler::confirmedDescriptorWrite);
        connect(service, &QLowEnergyService::errorOccurred,
                this, &Qt6Rockchip::Bluetooth::BluetoothHandler::execution);

        service->discoverDetails();
    } else {
        emit resolve("Heart Rate Service not found.");
    }
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::execution(QLowEnergyService::ServiceError error)
{
    emit reject(tr("LowEnergy service error[%0]").arg(error));
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::serviceStateChanged(QLowEnergyService::ServiceState newState)
{
    switch (newState) {
    case QLowEnergyService::RemoteServiceDiscovering:
        emit resolve(tr("Discovering services..."));
        break;
    case QLowEnergyService::RemoteServiceDiscovered:
    {
        emit resolve(tr("Service discovered."));
        const QLowEnergyCharacteristic hrChar =
            service->characteristic(QBluetoothUuid(QBluetoothUuid::CharacteristicType::HeartRateMeasurement));
        if (!hrChar.isValid()) {
            emit reject("HR Data not found.");
            break;
        }

        notificationDescriptor = hrChar.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
        if (notificationDescriptor.isValid())
            service->writeDescriptor(notificationDescriptor, QByteArray::fromHex("0100"));

        break;
    }
    default:
        //nothing for now
        break;
    }

    emit aliveChanged();
}

void Qt6Rockchip::Bluetooth::BluetoothHandler::updateHeartRateValue(const QLowEnergyCharacteristic &c,
                                                                    const QByteArray &value)
{
    // ignore any other characteristic change -> shouldn't really happen though
    if (c.uuid() != QBluetoothUuid(QBluetoothUuid::CharacteristicType::HeartRateMeasurement))
        return;

    auto data = reinterpret_cast<const quint8 *>(value.constData());
    quint8 flags = *data;

    //Heart Rate
    int hrvalue = 0;
    if (flags & 0x1) // HR 16 bit? otherwise 8 bit
        hrvalue = static_cast<int>(qFromLittleEndian<quint16>(data[1]));
    else
        hrvalue = static_cast<int>(data[1]);


}

void Qt6Rockchip::Bluetooth::BluetoothHandler::confirmedDescriptorWrite(const QLowEnergyDescriptor &d,
                                                                        const QByteArray &value)
{
    if (d.isValid() && d == notificationDescriptor && value == QByteArray::fromHex("0000")) {
        //disabled notifications -> assume disconnect intent
        controller->disconnectFromDevice();
        delete service;
        service = nullptr;
    }
}
