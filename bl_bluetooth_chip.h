/**
 * @file bl_bluetooth_chip.h
 * @brief BL Bluetooth Chip Interface for Connection and Audio Playback
 * 
 * This header provides the interface for the BL Bluetooth chip
 * for Bluetooth connection and audio playback capabilities.
 */

#ifndef BL_BLUETOOTH_CHIP_H
#define BL_BLUETOOTH_CHIP_H

#include <string>
#include <vector>
#include <functional>

namespace BL {

/**
 * @brief Bluetooth device information structure
 */
struct BluetoothDevice {
    std::string address;        // MAC address
    std::string name;           // Device name
    int signalStrength;         // Signal strength (RSSI)
};

/**
 * @brief Bluetooth connection state
 */
enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    DISCONNECTING
};

/**
 * @brief Audio playback state
 */
enum class PlaybackState {
    STOPPED,
    PLAYING,
    PAUSED
};

/**
 * @brief Bluetooth connection callback function type
 */
using ConnectionCallback = std::function<void(ConnectionState, const std::string&)>;

/**
 * @brief Audio playback callback function type
 */
using PlaybackCallback = std::function<void(PlaybackState)>;

/**
 * @brief BL Bluetooth Chip class for connection and audio playback
 */
class BluetoothChip {
public:
    /**
     * @brief Constructor
     */
    BluetoothChip();
    
    /**
     * @brief Destructor
     */
    ~BluetoothChip();
    
    /**
     * @brief Initialize the Bluetooth chip
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Start scanning for nearby Bluetooth devices
     * @param durationSeconds Scan duration in seconds
     * @return true if scan started successfully, false otherwise
     */
    bool startScan(int durationSeconds = 10);
    
    /**
     * @brief Stop scanning for devices
     * @return true if stopped successfully, false otherwise
     */
    bool stopScan();
    
    /**
     * @brief Get list of discovered devices
     * @return Vector of discovered Bluetooth devices
     */
    std::vector<BluetoothDevice> getDiscoveredDevices() const;
    
    /**
     * @brief Connect to a Bluetooth device
     * @param deviceAddress MAC address of the device to connect
     * @return true if connection initiated successfully, false otherwise
     */
    bool connect(const std::string& deviceAddress);
    
    /**
     * @brief Disconnect from the current device
     * @return true if disconnection initiated successfully, false otherwise
     */
    bool disconnect();
    
    /**
     * @brief Get current connection state
     * @return Current connection state
     */
    ConnectionState getConnectionState() const;
    
    /**
     * @brief Get currently connected device address
     * @return Device address or empty string if not connected
     */
    std::string getConnectedDevice() const;
    
    /**
     * @brief Play audio through Bluetooth
     * @param audioData Audio data buffer
     * @param size Size of audio data
     * @return true if playback started successfully, false otherwise
     */
    bool playAudio(const unsigned char* audioData, size_t size);
    
    /**
     * @brief Play audio file through Bluetooth
     * @param filename Path to audio file
     * @return true if playback started successfully, false otherwise
     */
    bool playAudioFile(const std::string& filename);
    
    /**
     * @brief Pause audio playback
     * @return true if paused successfully, false otherwise
     */
    bool pausePlayback();
    
    /**
     * @brief Resume audio playback
     * @return true if resumed successfully, false otherwise
     */
    bool resumePlayback();
    
    /**
     * @brief Stop audio playback
     * @return true if stopped successfully, false otherwise
     */
    bool stopPlayback();
    
    /**
     * @brief Get current playback state
     * @return Current playback state
     */
    PlaybackState getPlaybackState() const;
    
    /**
     * @brief Set volume level
     * @param level Volume level (0-100)
     * @return true if set successfully, false otherwise
     */
    bool setVolume(int level);
    
    /**
     * @brief Get current volume level
     * @return Current volume level
     */
    int getVolume() const;
    
    /**
     * @brief Set connection callback function
     * @param callback Callback function for connection state changes
     */
    void setConnectionCallback(ConnectionCallback callback);
    
    /**
     * @brief Set playback callback function
     * @param callback Callback function for playback state changes
     */
    void setPlaybackCallback(PlaybackCallback callback);
    
    /**
     * @brief Check if the chip is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;

private:
    class Impl;
    Impl* pImpl;
};

} // namespace BL

#endif // BL_BLUETOOTH_CHIP_H
