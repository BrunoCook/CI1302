# CI1302 + BL Bluetooth 快速参考指南

## 快速开始

### 编译
```bash
make
```

### 运行
```bash
./ci1302_bluetooth_system
```

### 清理
```bash
make clean
```

## 核心API快速参考

### CI1302 语音芯片

```cpp
#include "ci1302_voice_chip.h"

CI1302::VoiceChip chip;
chip.initialize();                              // 初始化
chip.addCommand({1, "播放", "Play"});            // 添加命令
chip.setCallback(callback);                     // 设置回调
chip.setSensitivity(75);                        // 设置灵敏度
chip.startRecognition();                        // 开始识别
chip.stopRecognition();                         // 停止识别
```

### BL 蓝牙芯片

```cpp
#include "bl_bluetooth_chip.h"

BL::BluetoothChip bt;
bt.initialize();                                // 初始化
bt.startScan(10);                               // 扫描设备
auto devices = bt.getDiscoveredDevices();       // 获取设备列表
bt.connect(devices[0].address);                 // 连接设备
bt.setVolume(70);                               // 设置音量
bt.playAudio(data, size);                       // 播放音频
bt.pausePlayback();                             // 暂停
bt.resumePlayback();                            // 继续
bt.stopPlayback();                              // 停止
bt.disconnect();                                // 断开连接
```

## 预配置语音命令

| 命令 | 功能 |
|------|------|
| 播放 | 播放音频 |
| 暂停 | 暂停播放 |
| 停止 | 停止播放 |
| 音量增加 | 增加音量 |
| 音量减小 | 减少音量 |
| 连接蓝牙 | 扫描并连接蓝牙设备 |
| 断开蓝牙 | 断开蓝牙连接 |

## 项目文件

| 文件 | 说明 |
|------|------|
| ci1302_voice_chip.h | CI1302语音芯片头文件 |
| ci1302_voice_chip.cpp | CI1302语音芯片实现 |
| bl_bluetooth_chip.h | BL蓝牙芯片头文件 |
| bl_bluetooth_chip.cpp | BL蓝牙芯片实现 |
| main.cpp | 主程序（完整演示） |
| example_usage.cpp | 使用示例 |
| Makefile | 构建配置 |

## 编译要求

- C++11 或更高
- pthread 支持
- GCC/Clang 编译器

## 注意事项

- 本实现为模拟实现，实际硬件需要适配
- 线程安全设计，支持并发操作
- 使用PIMPL模式，提高封装性
- 完整的回调机制支持事件驱动
