#pragma once

// C++ program to implement a basic logging system.

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
//using namespace std;

// Enum to represent log levels
enum class LogLevel { DEBUGG, INFO, WARNING, ERROR, CRITICAL };//DEBUGG instead of DEBUG as DEBUG is a defined value used for defining compilation of code in debug mode
enum class log_output_enum { consul_only, file_only, console_file };

class afw_Logger_class {
public:
    // Constructor: Opens the log file in append mode
    //afw_Logger_class() {}
    afw_Logger_class(const std::string& filename = "vw_log.txt", bool append = false)
    {
        if (!append) {
            if (std::filesystem::exists(filename)) {
                std::filesystem::remove(filename);
            }
        }

        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "ERROR :: Could not open a log file to write log data to.\n Can only output to Console.\n" << std::endl;
        }

        logger_file_initialised = true;
    }

    // Destructor: Closes the log file
    ~afw_Logger_class() { logFile.close(); }

    // Logs a message with a given log level to both consule and file
    void log(LogLevel level, const std::string& message, log_output_enum log_output = log_output_enum::console_file)
    {
        // Get current timestamp
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

        // Create log entry
        std::ostringstream logEntry;
        logEntry << "[" << timestamp << "] " << levelToString(level) << ": " << message << std::endl;

        // Output to console
        if(log_output == log_output_enum::consul_only || log_output == log_output_enum::console_file)
        std::cout << logEntry.str();

        // Output to log file

        if (log_output == log_output_enum::file_only || log_output == log_output_enum::console_file){
            if (logFile.is_open()) {
                logFile << logEntry.str();
                logFile.flush(); // Ensure immediate write to file
            }
        }
    }

    bool logger_file_initialised = false;

private:
    std::ofstream logFile; // File stream for the log file

    // Converts log level to a string for output
    std::string levelToString(LogLevel level)
    {
        switch (level) {
        case LogLevel::DEBUGG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARNING:  return "WARNING";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default:       return "UNKNOWN";
        }
    }
};
