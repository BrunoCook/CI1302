/**
 * @file ci1302_voice_chip.h
 * @brief CI1302 Voice Chip Offline Voice Recognition Interface
 * 
 * This header provides the interface for the CI1302 voice chip
 * for offline voice recognition capabilities.
 */

#ifndef CI1302_VOICE_CHIP_H
#define CI1302_VOICE_CHIP_H

#include <string>
#include <vector>
#include <functional>

namespace CI1302 {

/**
 * @brief Voice command structure
 */
struct VoiceCommand {
    int id;                     // Command ID
    std::string keyword;        // Voice keyword
    std::string description;    // Command description
};

/**
 * @brief Voice recognition callback function type
 */
using VoiceCallback = std::function<void(const VoiceCommand&)>;

/**
 * @brief CI1302 Voice Chip class for offline voice recognition
 */
class VoiceChip {
public:
    /**
     * @brief Constructor
     */
    VoiceChip();
    
    /**
     * @brief Destructor
     */
    ~VoiceChip();
    
    /**
     * @brief Initialize the voice chip
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Start voice recognition
     * @return true if started successfully, false otherwise
     */
    bool startRecognition();
    
    /**
     * @brief Stop voice recognition
     * @return true if stopped successfully, false otherwise
     */
    bool stopRecognition();
    
    /**
     * @brief Add a voice command to the recognition list
     * @param command Voice command to add
     * @return true if added successfully, false otherwise
     */
    bool addCommand(const VoiceCommand& command);
    
    /**
     * @brief Remove a voice command from the recognition list
     * @param commandId ID of the command to remove
     * @return true if removed successfully, false otherwise
     */
    bool removeCommand(int commandId);
    
    /**
     * @brief Get all registered voice commands
     * @return Vector of all voice commands
     */
    std::vector<VoiceCommand> getCommands() const;
    
    /**
     * @brief Set callback function for voice recognition
     * @param callback Callback function to be called when voice is recognized
     */
    void setCallback(VoiceCallback callback);
    
    /**
     * @brief Check if the chip is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;
    
    /**
     * @brief Check if recognition is running
     * @return true if running, false otherwise
     */
    bool isRecognitionRunning() const;
    
    /**
     * @brief Set sensitivity level for voice recognition
     * @param level Sensitivity level (0-100)
     * @return true if set successfully, false otherwise
     */
    bool setSensitivity(int level);
    
    /**
     * @brief Get current sensitivity level
     * @return Current sensitivity level
     */
    int getSensitivity() const;

private:
    class Impl;
    Impl* pImpl;
};

} // namespace CI1302

#endif // CI1302_VOICE_CHIP_H
