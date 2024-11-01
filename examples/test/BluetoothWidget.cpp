#include "BluetoothWidget.h"
#include <QStringListModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListView>

BluetoothWidget::BluetoothWidget(QWidget *parent)
    : QWidget{parent}
{
    bluetoothManager=new Qt6Rockchip::Bluetooth::BluetoothManager (this);
    bluetoothModel = new  Qt6Rockchip::Bluetooth::BluetoothItemModel(this);

    editlogger= new QTextEdit;
    editlogger->setFontPointSize(12);
    editlogger->setPlaceholderText("bluetooth write log");
    QListView* listView = new QListView;
    QPushButton* clearButton = new QPushButton("clear");
    QPushButton* searchButton = new QPushButton("search");
    QPushButton* connectButton = new QPushButton("connect");
    QPushButton* disconnectButton = new QPushButton("disconnect");
    QVBoxLayout* mainlayout = new QVBoxLayout;
    QHBoxLayout* toplayout = new QHBoxLayout;
    QHBoxLayout* bodylayout = new QHBoxLayout;
    QVBoxLayout* clrlayout = new QVBoxLayout;

    connect(bluetoothManager,&Qt6Rockchip::Bluetooth::BluetoothManager::CharacteristichSignal,
            [=](const QString &tybes){
                qDebug() << "BluetoothManager::CharacteristichSignal" << tybes;
            });

    connect(bluetoothManager,&Qt6Rockchip::Bluetooth::BluetoothManager::ControllerStateSignal,
            [=](const QLowEnergyController::ControllerState &state){
                qDebug() << "QLowEnergyController::ControllerState" << state;
            });

    connect(bluetoothManager,&Qt6Rockchip::Bluetooth::BluetoothManager::SearchSignal,
            [=](const QBluetoothDeviceInfo & info){
        bluetoothModel->push(info);
    });

    connect(bluetoothManager,&Qt6Rockchip::Bluetooth::BluetoothManager::ErrorSignal,
            this,&BluetoothWidget::logger);

    connect(clearButton,&QPushButton::clicked,[=](bool checked){
        editlogger->clear();
    });
    connect(searchButton,&QPushButton::clicked,[=](bool checked){
        bluetoothName.clear();
        bluetoothManager->searchBluetooth();
    });
    connect(connectButton,&QPushButton::clicked,[=](bool checked){
        QModelIndex index = listView->currentIndex();
        QBluetoothDeviceInfo info = index.data(Qt::UserRole).value<QBluetoothDeviceInfo>();
        if(info.isValid()){
           bluetoothManager->connectBluetooth(info);
        }

    });

    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setEditTriggers(QAbstractItemView::DoubleClicked);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setModel(bluetoothModel);
    listView->setStyleSheet("QListView {"
                            "    background-color: #f0f0f0;"
                            "    color: #333;"
                            "    border: 1px solid #ddd;"
                            "}"
                            "QListView::item {"
                            "    padding: 5px;"
                            "}"
                            "QListView::item:selected {"
                            "    background-color: #3399ff;"
                            "    color: white;"
                            "}");

    clrlayout->addWidget(clearButton);
    clrlayout->addWidget(searchButton);
    clrlayout->addWidget(connectButton);
    clrlayout->addWidget(disconnectButton);

    toplayout->addWidget(listView);
    mainlayout->addLayout(toplayout);
    bodylayout->addWidget(editlogger);
    bodylayout->addLayout(clrlayout);
    mainlayout->addLayout(bodylayout);
    setLayout(mainlayout);
}

void BluetoothWidget::logger(const QString &log)
{
    if(!log.isEmpty()){
        editlogger->append(log);
    }
}
