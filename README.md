# CI1302 语音芯片 + BL 蓝牙芯片集成系统

CI1302 离线语音识别芯片与 BL 蓝牙芯片的 C++ 集成实现。

## 项目简介

本项目实现了 CI1302 语音芯片的离线语音识别功能，并集成 BL 蓝牙芯片实现蓝牙连接和音频播放。该系统支持语音命令控制蓝牙设备，实现完整的语音控制音频播放功能。

## 功能特性

### CI1302 语音芯片功能
- ✅ 离线语音识别
- ✅ 自定义语音命令
- ✅ 语音识别回调机制
- ✅ 灵敏度调节（0-100）
- ✅ 实时语音处理
- ✅ 多命令支持

### BL 蓝牙芯片功能
- ✅ 蓝牙设备扫描
- ✅ 蓝牙设备连接/断开
- ✅ 音频数据播放
- ✅ 音频文件播放
- ✅ 播放控制（播放/暂停/停止）
- ✅ 音量控制（0-100）
- ✅ 连接状态回调
- ✅ 播放状态回调

### 集成功能
- ✅ 语音控制蓝牙连接
- ✅ 语音控制音频播放
- ✅ 语音控制音量调节
- ✅ 完整的事件驱动架构

## 文件结构

```
CI1302/
├── ci1302_voice_chip.h         # CI1302 语音芯片头文件
├── ci1302_voice_chip.cpp       # CI1302 语音芯片实现
├── bl_bluetooth_chip.h         # BL 蓝牙芯片头文件
├── bl_bluetooth_chip.cpp       # BL 蓝牙芯片实现
├── main.cpp                    # 主应用程序
├── example_usage.cpp           # 使用示例
├── Makefile                    # 构建配置
└── README.md                   # 项目文档
```

## 编译和运行

### 系统要求
- C++11 或更高版本的编译器（GCC/Clang）
- pthread 库支持
- Linux/Unix 系统

### 编译项目

```bash
# 编译主程序
make

# 清理编译文件
make clean

# 编译并运行
make run
```

### 手动编译

```bash
# 编译主程序
g++ -std=c++11 -pthread -o ci1302_bluetooth_system main.cpp ci1302_voice_chip.cpp bl_bluetooth_chip.cpp

# 编译示例程序
g++ -std=c++11 -pthread -o example_usage example_usage.cpp ci1302_voice_chip.cpp bl_bluetooth_chip.cpp
```

### 运行程序

```bash
# 运行主程序
./ci1302_bluetooth_system

# 运行示例程序
./example_usage
```

## 使用方法

### 1. CI1302 语音芯片基本使用

```cpp
#include "ci1302_voice_chip.h"

// 创建语音芯片实例
CI1302::VoiceChip voiceChip;

// 初始化芯片
voiceChip.initialize();

// 添加语音命令
voiceChip.addCommand({1, "播放", "Play audio"});
voiceChip.addCommand({2, "暂停", "Pause audio"});
voiceChip.addCommand({3, "停止", "Stop audio"});

// 设置识别回调
voiceChip.setCallback([](const CI1302::VoiceCommand& cmd) {
    std::cout << "识别到命令: " << cmd.keyword << std::endl;
    // 执行相应操作
});

// 设置灵敏度
voiceChip.setSensitivity(75);

// 启动语音识别
voiceChip.startRecognition();

// ... 应用运行 ...

// 停止语音识别
voiceChip.stopRecognition();
```

### 2. BL 蓝牙芯片基本使用

```cpp
#include "bl_bluetooth_chip.h"

// 创建蓝牙芯片实例
BL::BluetoothChip bluetoothChip;

// 初始化芯片
bluetoothChip.initialize();

// 设置连接状态回调
bluetoothChip.setConnectionCallback(
    [](BL::ConnectionState state, const std::string& device) {
        std::cout << "连接状态: " << state << std::endl;
    }
);

// 扫描蓝牙设备
bluetoothChip.startScan(10);  // 扫描10秒
std::this_thread::sleep_for(std::chrono::seconds(11));

// 获取发现的设备
auto devices = bluetoothChip.getDiscoveredDevices();
for (const auto& device : devices) {
    std::cout << device.name << " - " << device.address << std::endl;
}

// 连接到设备
if (!devices.empty()) {
    bluetoothChip.connect(devices[0].address);
}

// 播放音频
unsigned char audioData[1024] = {0};
bluetoothChip.playAudio(audioData, sizeof(audioData));

// 或播放音频文件
bluetoothChip.playAudioFile("music.wav");

// 音量控制
bluetoothChip.setVolume(70);

// 播放控制
bluetoothChip.pausePlayback();
bluetoothChip.resumePlayback();
bluetoothChip.stopPlayback();

// 断开连接
bluetoothChip.disconnect();
```

### 3. 集成使用示例

```cpp
#include "ci1302_voice_chip.h"
#include "bl_bluetooth_chip.h"

int main() {
    CI1302::VoiceChip voiceChip;
    BL::BluetoothChip bluetoothChip;
    
    voiceChip.initialize();
    bluetoothChip.initialize();
    
    // 添加语音控制命令
    voiceChip.addCommand({1, "播放", "Play"});
    voiceChip.addCommand({2, "暂停", "Pause"});
    voiceChip.addCommand({3, "音量增加", "Volume Up"});
    
    // 设置语音识别回调，控制蓝牙播放
    voiceChip.setCallback([&bluetoothChip](const CI1302::VoiceCommand& cmd) {
        switch (cmd.id) {
            case 1:  // 播放
                unsigned char audio[1024] = {0};
                bluetoothChip.playAudio(audio, sizeof(audio));
                break;
            case 2:  // 暂停
                bluetoothChip.pausePlayback();
                break;
            case 3:  // 音量增加
                int vol = bluetoothChip.getVolume();
                bluetoothChip.setVolume(std::min(100, vol + 10));
                break;
        }
    });
    
    voiceChip.startRecognition();
    
    // 应用主循环
    // ...
    
    return 0;
}
```

## API 文档

### CI1302::VoiceChip 类

#### 主要方法

- `bool initialize()` - 初始化语音芯片
- `bool startRecognition()` - 开始语音识别
- `bool stopRecognition()` - 停止语音识别
- `bool addCommand(const VoiceCommand& command)` - 添加语音命令
- `bool removeCommand(int commandId)` - 删除语音命令
- `void setCallback(VoiceCallback callback)` - 设置识别回调
- `bool setSensitivity(int level)` - 设置灵敏度（0-100）
- `int getSensitivity() const` - 获取当前灵敏度
- `std::vector<VoiceCommand> getCommands() const` - 获取所有命令
- `bool isInitialized() const` - 检查是否已初始化
- `bool isRecognitionRunning() const` - 检查识别是否运行中

### BL::BluetoothChip 类

#### 主要方法

- `bool initialize()` - 初始化蓝牙芯片
- `bool startScan(int durationSeconds)` - 开始扫描设备
- `bool stopScan()` - 停止扫描
- `std::vector<BluetoothDevice> getDiscoveredDevices() const` - 获取发现的设备
- `bool connect(const std::string& deviceAddress)` - 连接设备
- `bool disconnect()` - 断开连接
- `bool playAudio(const unsigned char* audioData, size_t size)` - 播放音频数据
- `bool playAudioFile(const std::string& filename)` - 播放音频文件
- `bool pausePlayback()` - 暂停播放
- `bool resumePlayback()` - 恢复播放
- `bool stopPlayback()` - 停止播放
- `bool setVolume(int level)` - 设置音量（0-100）
- `int getVolume() const` - 获取当前音量
- `ConnectionState getConnectionState() const` - 获取连接状态
- `PlaybackState getPlaybackState() const` - 获取播放状态

## 支持的语音命令示例

主程序中预配置了以下中文语音命令：

- **播放** - 播放音频
- **暂停** - 暂停播放
- **停止** - 停止播放
- **音量增加** - 增加音量
- **音量减小** - 减少音量
- **连接蓝牙** - 扫描并连接蓝牙设备
- **断开蓝牙** - 断开当前蓝牙连接

可以根据需要自定义添加更多语音命令。

## 技术特点

1. **PIMPL 设计模式** - 提高封装性和编译速度
2. **线程安全** - 使用 mutex 保护共享资源
3. **异步处理** - 语音识别和蓝牙扫描在独立线程运行
4. **回调机制** - 事件驱动的设计，响应迅速
5. **模块化设计** - CI1302 和 BL 芯片完全独立，易于维护

## 注意事项

1. 本实现是模拟实现，实际硬件对接需要根据具体芯片的硬件接口和协议进行适配
2. 音频数据处理部分需要根据实际音频格式进行实现
3. 实际部署时需要考虑硬件资源限制和实时性要求
4. 建议在实际使用前进行充分的硬件测试

## 开发和扩展

### 添加新的语音命令

```cpp
voiceChip.addCommand({
    id,              // 命令ID（唯一）
    "关键词",         // 语音关键词
    "Command Desc"   // 命令描述
});
```

### 自定义回调处理

```cpp
voiceChip.setCallback([](const CI1302::VoiceCommand& cmd) {
    // 自定义处理逻辑
    if (cmd.id == YOUR_COMMAND_ID) {
        // 执行特定操作
    }
});
```

## 许可证

本项目遵循 MIT 许可证。

## 作者

BrunoCook

## 更新日志

### v1.0.0 (2025)
- 初始版本发布
- 实现 CI1302 语音芯片离线识别功能
- 实现 BL 蓝牙芯片连接和播放功能
- 完成两个芯片的集成演示
