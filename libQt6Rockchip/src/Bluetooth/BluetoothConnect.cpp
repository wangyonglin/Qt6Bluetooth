#include "BluetoothConnect.h"
#include <QLowEnergyController>

Qt6Rockchip::Bluetooth::BluetoothConnect::BluetoothConnect(QObject *parent)
    :QObject{parent}
{

}

Qt6Rockchip::Bluetooth::BluetoothConnect::~BluetoothConnect()
{

}






void Qt6Rockchip::Bluetooth::BluetoothConnect::create(const QBluetoothUuid &service)
{
    // 创建服务
    if(!lowenergyController)return;
    this->LEService = this->lowenergyController->createServiceObject(service,this);
    if (this->LEService == NULL)
    {
        emit reject(QString::fromLocal8Bit("创建蓝牙服务失败！"));
    } else {
        emit resolve(QString::fromLocal8Bit("创建蓝牙服务成功！"));

        //监听服务状态变化
        connect(this->LEService,&QLowEnergyService::stateChanged,
                this,&Qt6Rockchip::Bluetooth::BluetoothConnect::stateChanged);

        //服务的characteristic变化，有数据传来
        connect(this->LEService,&QLowEnergyService::characteristicChanged,
                this,&Qt6Rockchip::Bluetooth::BluetoothConnect::characteristicChanged);
        //描述符成功被写

        connect(this->LEService,&QLowEnergyService::characteristicRead,
                this,&Qt6Rockchip::Bluetooth::BluetoothConnect::characteristicRead);
        //
        connect(this->LEService, &QLowEnergyService::errorOccurred,
                this, &Qt6Rockchip::Bluetooth::BluetoothConnect::execution);


        QThread::msleep(1000);
        //服务详情发现函数
        this->LEService->discoverDetails();
    }

}

void Qt6Rockchip::Bluetooth::BluetoothConnect::cleanup()
{

}

void Qt6Rockchip::Bluetooth::BluetoothConnect::init(QLowEnergyController *lowenergyController)
{
    this->lowenergyController=lowenergyController;
}

void Qt6Rockchip::Bluetooth::BluetoothConnect::stateChanged(QLowEnergyService::ServiceState newState)
{
    Q_UNUSED(newState);
    QLowEnergyCharacteristic bleCharacteristic;
    //发现服务
    if(this->LEService->state() == QLowEnergyService::ServiceDiscovered){
      listLECharacteristic = this->LEService->characteristics();
        for(int i=0; i<listLECharacteristic.size(); i++){
            bleCharacteristic = listLECharacteristic.at(i);
            if(bleCharacteristic.isValid()){
              this->LEDescriptor = bleCharacteristic.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
                if(this->LEDescriptor.isValid()){
                    LEService->writeDescriptor(this->LEDescriptor,QByteArray::fromHex("0100"));
                }

                if (bleCharacteristic.properties() & QLowEnergyCharacteristic::WriteNoResponse || bleCharacteristic.properties() & QLowEnergyCharacteristic::Write)
                {
                    this->LECharacteristic = bleCharacteristic;
                }
            }
        }
    }
}

void Qt6Rockchip::Bluetooth::BluetoothConnect::characteristicChanged(const QLowEnergyCharacteristic &characteristic,const QByteArray &value)
{
    Q_UNUSED(characteristic);

    int len = value.size();
    if(len > 0){
        emit resolve(QString("rec:").append(value.toHex()));
    }


}

void Qt6Rockchip::Bluetooth::BluetoothConnect::characteristicRead(const QLowEnergyCharacteristic &info,
                                                            const QByteArray &value)
{ qDebug() << value.toStdString();
    emit reader(info,value);
}

void Qt6Rockchip::Bluetooth::BluetoothConnect::characteristicWritten(const QLowEnergyCharacteristic &info,
                                                               const QByteArray &value)
{   qDebug() << value.toStdString();
     emit reader(info,value);
}

void Qt6Rockchip::Bluetooth::BluetoothConnect::execution(QLowEnergyService::ServiceError error)
{
    emit reject(tr("蓝牙服务错误:%0").arg(error));

}
