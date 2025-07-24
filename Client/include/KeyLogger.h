#ifndef KEY_LOGGER_H
#define KEY_LOGGER_H
using namespace std;

#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>

/**
 * This class implements a simple key logger that runs in a separate thread.
 * It logs keystrokes to a buffer and allows retrieval of the logged keys.
 */
class KeyLogger {

public:

    // Constructor and destructor
    KeyLogger();
    ~KeyLogger();

    // Copy constructor and assignment operator
    KeyLogger(const KeyLogger&);
    // Assignment operator
    KeyLogger& operator=(const KeyLogger&);

    // Start and stop the key logger
    void start();
    void stop();

    // Retrieve the logged keys as a string
    string getLoggedKeys();

private:

    // The main loop for logging keys
    void logLoop();
    atomic<bool> running_;
    thread loggingThread_;
    string buffer_;
    mutex bufferMutex_;
};

#endif