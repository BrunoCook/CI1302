/**
 * @file example_usage.cpp
 * @brief Example usage of CI1302 Voice Chip and BL Bluetooth Chip
 * 
 * This example demonstrates how to use the CI1302 and BL chip APIs
 * in a simple application.
 */

#include "ci1302_voice_chip.h"
#include "bl_bluetooth_chip.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== CI1302 + BL Bluetooth Example ===" << std::endl;
    std::cout << std::endl;
    
    // Example 1: Basic Voice Recognition Setup
    std::cout << "Example 1: Basic Voice Recognition Setup" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    CI1302::VoiceChip voice;
    voice.initialize();
    
    // Add voice commands
    voice.addCommand({1, "开始", "Start command"});
    voice.addCommand({2, "结束", "End command"});
    
    // Set callback
    voice.setCallback([](const CI1302::VoiceCommand& cmd) {
        std::cout << "Recognized: " << cmd.keyword << std::endl;
    });
    
    // Start recognition
    voice.startRecognition();
    std::cout << "Voice recognition started" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    voice.stopRecognition();
    std::cout << std::endl;
    
    // Example 2: Bluetooth Device Discovery
    std::cout << "Example 2: Bluetooth Device Discovery" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    
    BL::BluetoothChip bluetooth;
    bluetooth.initialize();
    
    // Scan for devices
    std::cout << "Scanning for Bluetooth devices..." << std::endl;
    bluetooth.startScan(5);
    std::this_thread::sleep_for(std::chrono::seconds(6));
    
    // Display found devices
    auto devices = bluetooth.getDiscoveredDevices();
    std::cout << "Found " << devices.size() << " device(s):" << std::endl;
    for (const auto& dev : devices) {
        std::cout << "  " << dev.name << " - " << dev.address << std::endl;
    }
    std::cout << std::endl;
    
    // Example 3: Bluetooth Connection and Audio Playback
    if (!devices.empty()) {
        std::cout << "Example 3: Bluetooth Connection and Audio Playback" << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
        
        // Connect to first device
        std::cout << "Connecting to: " << devices[0].name << std::endl;
        bluetooth.connect(devices[0].address);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Play audio
        std::cout << "Playing audio..." << std::endl;
        unsigned char audio[1024] = {0};
        bluetooth.playAudio(audio, sizeof(audio));
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // Control volume
        std::cout << "Adjusting volume..." << std::endl;
        bluetooth.setVolume(80);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // Stop playback
        bluetooth.stopPlayback();
        std::cout << "Playback stopped" << std::endl;
        
        // Disconnect
        bluetooth.disconnect();
        std::cout << "Disconnected" << std::endl;
        std::cout << std::endl;
    }
    
    // Example 4: Integrated Voice-Controlled Bluetooth
    std::cout << "Example 4: Integrated Voice-Controlled Bluetooth" << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    
    // Setup voice commands for Bluetooth control
    CI1302::VoiceChip voiceControl;
    BL::BluetoothChip btControl;
    
    voiceControl.initialize();
    btControl.initialize();
    
    voiceControl.addCommand({10, "播放音乐", "Play music"});
    voiceControl.addCommand({11, "停止音乐", "Stop music"});
    
    voiceControl.setCallback([&btControl](const CI1302::VoiceCommand& cmd) {
        std::cout << "Voice command: " << cmd.keyword << std::endl;
        
        if (cmd.id == 10) {
            // Play command
            if (btControl.getConnectionState() == BL::ConnectionState::CONNECTED) {
                unsigned char dummyAudio[512] = {0};
                btControl.playAudio(dummyAudio, sizeof(dummyAudio));
            } else {
                std::cout << "Not connected to Bluetooth device" << std::endl;
            }
        } else if (cmd.id == 11) {
            // Stop command
            btControl.stopPlayback();
        }
    });
    
    voiceControl.startRecognition();
    std::cout << "Voice-controlled Bluetooth system active" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    voiceControl.stopRecognition();
    
    std::cout << std::endl;
    std::cout << "=== Example Complete ===" << std::endl;
    
    return 0;
}
