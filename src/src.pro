TEMPLATE = lib
QT       += core gui opengl widgets bluetooth
CONFIG += c++11
#QT6LIBRARY_LIBRARY



linux-g++* {
    CONFIG(debug, debug|release){
        message(Debug build)
        TARGET = Qt6Bluetooth
        release
    }
    CONFIG(release, debug|release){
        message(Release build)
        TARGET = Qt6Bluetooth
    }
    DEFINES += CSM_TARGET_LINUX_GL
}

DISTFILES += \
    README.md \

HEADERS += \
    BluetoothExport.h \
    BluetoothItemModel.h \
    BluetoothManager.h \
    BluetoothObject.h \
    BluetoothSearch.h \
    BluetoothService.h

SOURCES += \
    BluetoothItemModel.cpp \
    BluetoothManager.cpp \
    BluetoothObject.cpp \
    BluetoothSearch.cpp \
    BluetoothService.cpp









