#include "KeyLogger.h"
#include <chrono>
#include <thread>

KeyLogger::KeyLogger() : running_(false) {}

KeyLogger::KeyLogger(const KeyLogger& other)
    : running_(false), buffer_(other.buffer_) {}

KeyLogger& KeyLogger::operator=(const KeyLogger& other) {
    if (this != &other) {
        stop();
        buffer_ = other.buffer_;
    }
    return *this;
}

KeyLogger::~KeyLogger() {
    stop();
}

void KeyLogger::start() {
    if (running_) return;
    running_ = true;
    loggingThread_ = std::thread(&KeyLogger::logLoop, this);
}

void KeyLogger::stop() {
    if (!running_) return;
    running_ = false;
    if (loggingThread_.joinable())
        loggingThread_.join();
}

std::string KeyLogger::getLoggedKeys() {
    std::lock_guard<std::mutex> lock(bufferMutex_);
    std::string copy = buffer_;
    buffer_.clear();
    return copy;
}

void KeyLogger::logLoop() {
    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(bufferMutex_);
        buffer_ += "[KEY_SIMULATED]";
    }
}
