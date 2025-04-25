# Logger Documentation

## Overview

The Logger class provides a comprehensive logging system using the Quill library. It supports multiple log levels and can output logs to both console and file simultaneously. The logging system is implemented as a singleton, ensuring consistent logging throughout the application.

## Log Levels

The logger supports five different log levels:

1. **Debug** - Detailed debugging information useful during development
2. **Info** - General operational messages highlighting application progress
3. **Warn** - Potentially harmful situations that are not errors
4. **Error** - Error events that might still allow the application to continue
5. **None** - Disables all logging

## Usage

### Initialization

Before using the logger, it must be initialized with a log level and optional file path:

```cpp
// Initialize with console logging only
Logger::init("info", "");

// Initialize with both console and file logging
Logger::init("debug", "logs/app.log");
```

### Logging Messages

Log messages can be written using the Quill logging macros:

```cpp
// Get the logger instance
auto logger = Logger::logger;

// Log messages at different levels
LOG_DEBUG(logger, "Debug information");
LOG_INFO(logger, "General information");
LOG_WARN(logger, "Warning message");
LOG_ERROR(logger, "Error occurred");
```

### Formatting Messages

The logger supports formatted messages using the fmt library:

```cpp
LOG_INFO(logger, "Processing file: {}", filename);
LOG_ERROR(logger, "Failed to process {}: {}", filename, errorMessage);
```

## Configuration

### Log Level Control

The log level is set during initialization and can be one of:
- "debug" - Most verbose, shows all messages
- "info" - Shows informational messages and above
- "warn" - Shows warnings and errors
- "error" - Shows only errors
- "none" - Disables all logging

### Output Format

By default, logs are written in the following format:
- Console: Colored output with timestamp and log level
- File: Plain text with timestamp and log level

## Best Practices

1. Always initialize the logger before use
2. Use appropriate log levels for different types of messages
3. Use formatted messages for dynamic content
4. Consider the log level carefully in production environments
5. Use file logging for persistent storage of important logs

## Example

```cpp
// Initialize logger with both console and file output
Logger::init("info", "logs/app.log");

// Get the logger instance
auto logger = Logger::logger;

// Log application start
LOG_INFO(logger, "Application started");

try {
    // Some operation
    LOG_DEBUG(logger, "Processing data: {}", data);
} catch (const std::exception& e) {
    LOG_ERROR(logger, "Operation failed: {}", e.what());
}
```
