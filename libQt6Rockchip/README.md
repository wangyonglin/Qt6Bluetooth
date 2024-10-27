# ArchLinux安装使用蓝牙之基础

一、安装

1.安装蓝牙并启动蓝牙服务
```
$ sudo pacman -S bluez bluez-utils
$ sudo systemctl enable bluetooth.service
$ sudo systemctl start bluetooth.service
```

二、使用Bluetoothctl进入蓝牙控制台

Bluez是Linux系统上的蓝牙协议栈，自带了很多有用的工具，Bluetoothctl就是其一。
```
$ bluetoothctl --help          查看帮助命令
$ bluetoothctl -v              查看蓝牙版本
```

三、使用Bluetoothctl连接蓝牙案例
```
$ bluetoothctl            进入蓝牙管理工具环境
[bluetooth]# power on                打开蓝牙
[bluetooth]# agent on                开启代理
[bluetooth]# scan on                 扫描蓝牙设备
[bluetooth]# pair DD:0D:30:9E:F9:B9       配对该设备
[bluetooth]# trust DD:0D:30:9E:F9:B9      信任该设备
[bluetooth]# connect DD:0D:30:9E:F9:B9       连接该设备
[bluetooth]# disconnect DD:0D:30:9E:F9:B9    断开蓝牙

[bluetooth]# help                   查看帮助信息
[bluetooth]# show                   查看本机蓝牙信息
[bluetooth]# discoverable yes       设置蓝牙可被发现
[bluetooth]# info DD:0D:30:9E:F9:B9      查看该蓝牙设备的信息
[bluetooth]# quit                   退出
```

四、安装蓝牙音频
```
$ sudo pacman -S pulseaudio-bluetooth
```

五、蓝牙图形化管理工具安装
```
$ sudo pacman -S bluedevil
或
$ sudo pacman -S blueman

```
