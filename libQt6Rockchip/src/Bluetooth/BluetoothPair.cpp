#include "BluetoothPair.h"
Qt6Rockchip::Bluetooth::BluetoothPair::BluetoothPair(QObject *parent)
    :QObject{parent}
{

}

QLowEnergyController* Qt6Rockchip::Bluetooth::BluetoothPair::pairing(const QBluetoothDeviceInfo & deviceinfo)
{
    this->bluetoothDeviceInfo=deviceinfo;
    if(this->bluetoothDeviceInfo.isValid()){
        this->lowenergyController = QLowEnergyController::createCentral(this->bluetoothDeviceInfo,this);
        if (this->lowenergyController == NULL)
        {
            emit reject(QString::fromLocal8Bit("创建蓝牙控制器失败！"));
            return this->lowenergyController;
        }
        else
        {
            emit resolve(QString::fromLocal8Bit("创建蓝牙控制器成功！"));
        }

        connect(this->lowenergyController, &QLowEnergyController::connected,
                this, &Qt6Rockchip::Bluetooth::BluetoothPair::connected);
        connect(this->lowenergyController, &QLowEnergyController::disconnected,
                this, &Qt6Rockchip::Bluetooth::BluetoothPair::disconnected);
        connect(this->lowenergyController,  &QLowEnergyController::discoveryFinished,
                this, &Qt6Rockchip::Bluetooth::BluetoothPair::finished);
        connect(this->lowenergyController, &QLowEnergyController::errorOccurred,
                this, &Qt6Rockchip::Bluetooth::BluetoothPair::execution);
        connect(this->lowenergyController, &QLowEnergyController::serviceDiscovered,
                this, &Qt6Rockchip::Bluetooth::BluetoothPair::discovered);
        // 开始计时控制器的连接超时
        LECTimer.start(10000);
        // 发起连接
        this->lowenergyController->connectToDevice();
    }
    return this->lowenergyController;
}

void Qt6Rockchip::Bluetooth::BluetoothPair::connected()
{
    // 停止连接计时
    this->LECTimer.stop();
    emit resolve(QString::fromLocal8Bit("蓝牙控制器连接成功，开始搜索蓝牙服务！"));
    this->uuids.clear();
    QThread::msleep(1000);
    //搜索服务
    this->lowenergyController->discoverServices();
}

void Qt6Rockchip::Bluetooth::BluetoothPair::disconnected()
{
    if(this->LECTimer.isActive())
        this->LECTimer.stop();

    if(this->lowenergyController) {
        this->lowenergyController->disconnectFromDevice();
        delete this->lowenergyController;
        this->lowenergyController = NULL;
    }
}

void Qt6Rockchip::Bluetooth::BluetoothPair::finished()
{
    emit resolve(QString::fromLocal8Bit("蓝牙服务搜索结束！"));
    QThread::msleep(1000);
    emit sender(this->uuids);
}

void Qt6Rockchip::Bluetooth::BluetoothPair::discovered(const QBluetoothUuid &newService)
{
    emit resolve(tr("发现[%0]蓝牙服务-UUID:%1").arg(this->bluetoothDeviceInfo.name()).arg(newService.toString()));
    emit paired(this->lowenergyController,newService);
    this->uuids.append(newService);
}

void Qt6Rockchip::Bluetooth::BluetoothPair::stateChanged(QLowEnergyController::ControllerState state)
{
    emit resolve(tr("控制器状态:{%0}").arg(state));
}

void Qt6Rockchip::Bluetooth::BluetoothPair::execution(QLowEnergyController::Error newError)
{
    emit reject(tr("控制器错误:{%0}").arg(newError));
    // if(newError==QLowEnergyController::Error::UnknownError){
    //     this->lowenergyController->discoverServices();
    //     this->lowenergyController->connectToDevice();
    // }

}
void Qt6Rockchip::Bluetooth::BluetoothPair::cleanup(){
    this->lowenergyController->disconnectFromDevice();
}

