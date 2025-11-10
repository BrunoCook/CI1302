/**
 * @file main.cpp
 * @brief Main application integrating CI1302 Voice Chip and BL Bluetooth Chip
 * 
 * This application demonstrates the integration of CI1302 voice chip for
 * offline voice recognition and BL Bluetooth chip for wireless audio playback.
 */

#include "ci1302_voice_chip.h"
#include "bl_bluetooth_chip.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

// Global flag for graceful shutdown
std::atomic<bool> running(true);

// Signal handler for clean exit
void signalHandler(int signal) {
    std::cout << "\n[MAIN] Received signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

// Convert connection state to string
std::string connectionStateToString(BL::ConnectionState state) {
    switch (state) {
        case BL::ConnectionState::DISCONNECTED: return "DISCONNECTED";
        case BL::ConnectionState::CONNECTING: return "CONNECTING";
        case BL::ConnectionState::CONNECTED: return "CONNECTED";
        case BL::ConnectionState::DISCONNECTING: return "DISCONNECTING";
        default: return "UNKNOWN";
    }
}

// Convert playback state to string
std::string playbackStateToString(BL::PlaybackState state) {
    switch (state) {
        case BL::PlaybackState::STOPPED: return "STOPPED";
        case BL::PlaybackState::PLAYING: return "PLAYING";
        case BL::PlaybackState::PAUSED: return "PAUSED";
        default: return "UNKNOWN";
    }
}

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "CI1302 Voice Chip + BL Bluetooth System" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Setup signal handlers
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    // Initialize CI1302 Voice Chip
    std::cout << "--- Initializing CI1302 Voice Chip ---" << std::endl;
    CI1302::VoiceChip voiceChip;
    if (!voiceChip.initialize()) {
        std::cerr << "[ERROR] Failed to initialize voice chip" << std::endl;
        return 1;
    }
    std::cout << std::endl;
    
    // Initialize BL Bluetooth Chip
    std::cout << "--- Initializing BL Bluetooth Chip ---" << std::endl;
    BL::BluetoothChip bluetoothChip;
    if (!bluetoothChip.initialize()) {
        std::cerr << "[ERROR] Failed to initialize Bluetooth chip" << std::endl;
        return 1;
    }
    std::cout << std::endl;
    
    // Setup Bluetooth callbacks
    bluetoothChip.setConnectionCallback(
        [](BL::ConnectionState state, const std::string& device) {
            std::cout << "[CALLBACK] Connection state changed: " 
                      << connectionStateToString(state) 
                      << " - Device: " << device << std::endl;
        }
    );
    
    bluetoothChip.setPlaybackCallback(
        [](BL::PlaybackState state) {
            std::cout << "[CALLBACK] Playback state changed: " 
                      << playbackStateToString(state) << std::endl;
        }
    );
    
    // Configure voice commands
    std::cout << "--- Configuring Voice Commands ---" << std::endl;
    voiceChip.addCommand({1, "播放", "Play audio"});
    voiceChip.addCommand({2, "暂停", "Pause audio"});
    voiceChip.addCommand({3, "停止", "Stop audio"});
    voiceChip.addCommand({4, "音量增加", "Increase volume"});
    voiceChip.addCommand({5, "音量减小", "Decrease volume"});
    voiceChip.addCommand({6, "连接蓝牙", "Connect Bluetooth"});
    voiceChip.addCommand({7, "断开蓝牙", "Disconnect Bluetooth"});
    std::cout << std::endl;
    
    // Setup voice recognition callback
    voiceChip.setCallback(
        [&bluetoothChip](const CI1302::VoiceCommand& command) {
            std::cout << "[VOICE ACTION] Command recognized: " << command.keyword 
                      << " (" << command.description << ")" << std::endl;
            
            // Execute corresponding action based on command
            switch (command.id) {
                case 1: // Play
                    if (bluetoothChip.getConnectionState() == BL::ConnectionState::CONNECTED) {
                        // Simulate playing audio data
                        unsigned char dummyAudio[1024] = {0};
                        bluetoothChip.playAudio(dummyAudio, sizeof(dummyAudio));
                    } else {
                        std::cout << "[VOICE ACTION] Cannot play - not connected to Bluetooth device" << std::endl;
                    }
                    break;
                    
                case 2: // Pause
                    bluetoothChip.pausePlayback();
                    break;
                    
                case 3: // Stop
                    bluetoothChip.stopPlayback();
                    break;
                    
                case 4: // Volume up
                    {
                        int currentVolume = bluetoothChip.getVolume();
                        bluetoothChip.setVolume(std::min(100, currentVolume + 10));
                    }
                    break;
                    
                case 5: // Volume down
                    {
                        int currentVolume = bluetoothChip.getVolume();
                        bluetoothChip.setVolume(std::max(0, currentVolume - 10));
                    }
                    break;
                    
                case 6: // Connect Bluetooth
                    std::cout << "[VOICE ACTION] Scanning for Bluetooth devices..." << std::endl;
                    bluetoothChip.startScan(5);
                    std::this_thread::sleep_for(std::chrono::seconds(6));
                    {
                        auto devices = bluetoothChip.getDiscoveredDevices();
                        if (!devices.empty()) {
                            std::cout << "[VOICE ACTION] Connecting to first discovered device..." << std::endl;
                            bluetoothChip.connect(devices[0].address);
                        } else {
                            std::cout << "[VOICE ACTION] No devices found" << std::endl;
                        }
                    }
                    break;
                    
                case 7: // Disconnect Bluetooth
                    bluetoothChip.disconnect();
                    break;
                    
                default:
                    std::cout << "[VOICE ACTION] Unknown command ID: " << command.id << std::endl;
                    break;
            }
        }
    );
    
    // Set voice recognition sensitivity
    voiceChip.setSensitivity(75);
    
    // Start voice recognition
    std::cout << "--- Starting Voice Recognition ---" << std::endl;
    if (!voiceChip.startRecognition()) {
        std::cerr << "[ERROR] Failed to start voice recognition" << std::endl;
        return 1;
    }
    std::cout << std::endl;
    
    // Demonstrate Bluetooth scanning
    std::cout << "--- Demonstrating Bluetooth Scan ---" << std::endl;
    bluetoothChip.startScan(5);
    std::this_thread::sleep_for(std::chrono::seconds(6));
    
    // Display discovered devices
    auto devices = bluetoothChip.getDiscoveredDevices();
    std::cout << "\nDiscovered " << devices.size() << " device(s):" << std::endl;
    for (const auto& device : devices) {
        std::cout << "  - " << device.name << " [" << device.address << "] "
                  << "RSSI: " << device.signalStrength << " dBm" << std::endl;
    }
    std::cout << std::endl;
    
    // Connect to first device if available
    if (!devices.empty()) {
        std::cout << "--- Connecting to First Device ---" << std::endl;
        bluetoothChip.connect(devices[0].address);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        std::cout << std::endl;
        
        // Test audio playback
        std::cout << "--- Testing Audio Playback ---" << std::endl;
        bluetoothChip.setVolume(60);
        unsigned char sampleAudio[2048] = {0};
        bluetoothChip.playAudio(sampleAudio, sizeof(sampleAudio));
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        bluetoothChip.pausePlayback();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        bluetoothChip.resumePlayback();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        bluetoothChip.stopPlayback();
        std::cout << std::endl;
    }
    
    // Main loop - keep running for voice recognition
    std::cout << "--- System Running ---" << std::endl;
    std::cout << "Voice recognition is active. Voice commands will be recognized automatically." << std::endl;
    std::cout << "Press Ctrl+C to stop the system." << std::endl;
    std::cout << std::endl;
    
    // Run for demonstration period or until interrupted
    int elapsed = 0;
    while (running && elapsed < 30) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        elapsed++;
    }
    
    // Cleanup
    std::cout << "\n--- Shutting Down System ---" << std::endl;
    voiceChip.stopRecognition();
    bluetoothChip.disconnect();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "System shutdown complete" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}
