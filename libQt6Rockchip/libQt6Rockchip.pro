TEMPLATE = lib
QT       += core gui opengl widgets bluetooth
CONFIG += c++11
#QT6LIBRARY_LIBRARY
include (src/Bluetooth/Bluetooth.pri)

linux-g++* {
    CONFIG(debug, debug|release){
        message(Debug build)
        TARGET = Qt6Rockchip
        release
    }
    CONFIG(release, debug|release){
        message(Release build)
        TARGET = Qt6Rockchip
    }
    DEFINES += CSM_TARGET_LINUX_GL
}

DISTFILES += \
    README.md \
    include/Qt6Rockchip/Bluetooth/BluetoothService





