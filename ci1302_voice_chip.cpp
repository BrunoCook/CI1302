/**
 * @file ci1302_voice_chip.cpp
 * @brief CI1302 Voice Chip Offline Voice Recognition Implementation
 */

#include "ci1302_voice_chip.h"
#include <iostream>
#include <algorithm>
#include <mutex>
#include <thread>
#include <chrono>

namespace CI1302 {

/**
 * @brief Internal implementation class (PIMPL pattern)
 */
class VoiceChip::Impl {
public:
    Impl() : initialized(false), recognitionRunning(false), sensitivity(70) {}
    
    ~Impl() {
        stopRecognition();
    }
    
    bool initialize() {
        std::lock_guard<std::mutex> lock(mutex);
        if (initialized) {
            std::cerr << "[CI1302] Already initialized" << std::endl;
            return true;
        }
        
        // Simulate hardware initialization
        std::cout << "[CI1302] Initializing voice chip..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        initialized = true;
        std::cout << "[CI1302] Voice chip initialized successfully" << std::endl;
        return true;
    }
    
    bool startRecognition() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!initialized) {
            std::cerr << "[CI1302] Chip not initialized" << std::endl;
            return false;
        }
        
        if (recognitionRunning) {
            std::cerr << "[CI1302] Recognition already running" << std::endl;
            return true;
        }
        
        recognitionRunning = true;
        std::cout << "[CI1302] Voice recognition started" << std::endl;
        
        // Start recognition thread
        recognitionThread = std::thread(&Impl::recognitionLoop, this);
        
        return true;
    }
    
    bool stopRecognition() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!recognitionRunning) {
            return true;
        }
        
        recognitionRunning = false;
        std::cout << "[CI1302] Voice recognition stopped" << std::endl;
        
        if (recognitionThread.joinable()) {
            mutex.unlock();
            recognitionThread.join();
            mutex.lock();
        }
        
        return true;
    }
    
    bool addCommand(const VoiceCommand& command) {
        std::lock_guard<std::mutex> lock(mutex);
        
        // Check if command ID already exists
        auto it = std::find_if(commands.begin(), commands.end(),
            [&command](const VoiceCommand& cmd) { return cmd.id == command.id; });
        
        if (it != commands.end()) {
            std::cerr << "[CI1302] Command ID " << command.id << " already exists" << std::endl;
            return false;
        }
        
        commands.push_back(command);
        std::cout << "[CI1302] Added command: " << command.keyword 
                  << " (ID: " << command.id << ")" << std::endl;
        return true;
    }
    
    bool removeCommand(int commandId) {
        std::lock_guard<std::mutex> lock(mutex);
        
        auto it = std::find_if(commands.begin(), commands.end(),
            [commandId](const VoiceCommand& cmd) { return cmd.id == commandId; });
        
        if (it == commands.end()) {
            std::cerr << "[CI1302] Command ID " << commandId << " not found" << std::endl;
            return false;
        }
        
        std::cout << "[CI1302] Removed command: " << it->keyword 
                  << " (ID: " << commandId << ")" << std::endl;
        commands.erase(it);
        return true;
    }
    
    std::vector<VoiceCommand> getCommands() const {
        std::lock_guard<std::mutex> lock(mutex);
        return commands;
    }
    
    void setCallback(VoiceCallback cb) {
        std::lock_guard<std::mutex> lock(mutex);
        callback = cb;
    }
    
    bool isInitialized() const {
        std::lock_guard<std::mutex> lock(mutex);
        return initialized;
    }
    
    bool isRecognitionRunning() const {
        std::lock_guard<std::mutex> lock(mutex);
        return recognitionRunning;
    }
    
    bool setSensitivity(int level) {
        std::lock_guard<std::mutex> lock(mutex);
        if (level < 0 || level > 100) {
            std::cerr << "[CI1302] Sensitivity level must be between 0 and 100" << std::endl;
            return false;
        }
        sensitivity = level;
        std::cout << "[CI1302] Sensitivity set to " << level << std::endl;
        return true;
    }
    
    int getSensitivity() const {
        std::lock_guard<std::mutex> lock(mutex);
        return sensitivity;
    }

private:
    void recognitionLoop() {
        std::cout << "[CI1302] Recognition loop started" << std::endl;
        
        while (true) {
            {
                std::lock_guard<std::mutex> lock(mutex);
                if (!recognitionRunning) {
                    break;
                }
            }
            
            // Simulate voice recognition process
            std::this_thread::sleep_for(std::chrono::seconds(2));
            
            // Check if there are commands to recognize
            std::lock_guard<std::mutex> lock(mutex);
            if (!commands.empty() && callback) {
                // Simulate recognition of a random command (for demonstration)
                // In real implementation, this would process actual voice input
                int index = std::rand() % commands.size();
                std::cout << "[CI1302] Voice recognized: " << commands[index].keyword << std::endl;
                callback(commands[index]);
            }
        }
        
        std::cout << "[CI1302] Recognition loop stopped" << std::endl;
    }
    
    bool initialized;
    bool recognitionRunning;
    int sensitivity;
    std::vector<VoiceCommand> commands;
    VoiceCallback callback;
    std::thread recognitionThread;
    mutable std::mutex mutex;
};

// VoiceChip public methods implementation

VoiceChip::VoiceChip() : pImpl(new Impl()) {}

VoiceChip::~VoiceChip() {
    delete pImpl;
}

bool VoiceChip::initialize() {
    return pImpl->initialize();
}

bool VoiceChip::startRecognition() {
    return pImpl->startRecognition();
}

bool VoiceChip::stopRecognition() {
    return pImpl->stopRecognition();
}

bool VoiceChip::addCommand(const VoiceCommand& command) {
    return pImpl->addCommand(command);
}

bool VoiceChip::removeCommand(int commandId) {
    return pImpl->removeCommand(commandId);
}

std::vector<VoiceCommand> VoiceChip::getCommands() const {
    return pImpl->getCommands();
}

void VoiceChip::setCallback(VoiceCallback callback) {
    pImpl->setCallback(callback);
}

bool VoiceChip::isInitialized() const {
    return pImpl->isInitialized();
}

bool VoiceChip::isRecognitionRunning() const {
    return pImpl->isRecognitionRunning();
}

bool VoiceChip::setSensitivity(int level) {
    return pImpl->setSensitivity(level);
}

int VoiceChip::getSensitivity() const {
    return pImpl->getSensitivity();
}

} // namespace CI1302
