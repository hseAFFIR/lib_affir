# Logger Documentation

## Overview

The Logger class provides a comprehensive logging system using the spdlog library. It supports multiple log levels and outputs logs to both console and file simultaneously. The logging level can be controlled via the `DEBUG` environment variable.

## Log Levels

The logger supports six different log levels, from most verbose to least:

1. **Trace** - Most detailed logging level, used for very detailed debugging information
2. **Debug** - Detailed debugging information useful during development
3. **Info** - General operational messages highlighting application progress
4. **Warn** - Potentially harmful situations that are not errors
5. **Error** - Error events that might still allow the application to continue
6. **Critical** - Very severe error events that may lead to application termination

## Usage

### Initialization

Before using the logger, it must be initialized with a log file path:

```cpp
Logger::init("path/to/log.txt");  // Custom log file path
// or
Logger::init();  // Uses default path "logs/log.txt"
```

### Logging Messages

Log messages can be written using the appropriate level method:

```cpp
Logger::trace("MyClass", "Detailed trace message");
Logger::debug("MyClass", "Debug information");
Logger::info("MyClass", "General information");
Logger::warn("MyClass", "Warning message");
Logger::error("MyClass", "Error occurred");
Logger::critical("MyClass", "Critical error!");
```

### Formatting Messages

The logger supports formatted messages using the fmt library:

```cpp
Logger::info("MyClass", "Processing file: {}", filename);
Logger::error("MyClass", "Failed to process {}: {}", filename, errorMessage);
```

## Configuration

### Log Level Control

The log level is controlled by the `DEBUG` environment variable:

- If `DEBUG=false`: Only info and higher level messages are logged
- Otherwise: All messages including trace and debug are logged

To set the environment variable:

```bash
# Windows PowerShell
$env:DEBUG="false"

# Windows Command Prompt
set DEBUG=false

# Linux/Mac
export DEBUG=false
```

### Output Format

Logs are written in the following format:
- Console: `%^%l: %Y-%m-%d %H:%M:%S [%n]: %v%$`
- File: `%l: %Y-%m-%d %H:%M:%S [%n]: %v`

Where:
- `%l` - Log level
- `%Y-%m-%d %H:%M:%S` - Timestamp
- `%n` - Logger name
- `%v` - Log message
- `%^` and `%$` - Color formatting (console only)

## Best Practices

1. Always initialize the logger before use
2. Use appropriate log levels for different types of messages
3. Include the class name in log messages for better traceability
4. Use formatted messages for dynamic content
5. Set `DEBUG=false` in production environments to reduce log verbosity

## Example

```cpp
// Initialize logger
Logger::init("logs/app.log");

// Log different types of messages
Logger::info("Main", "Application started");
Logger::debug("Database", "Connecting to database at {}", dbUrl);

try {
    // Some operation
} catch (const std::exception& e) {
    Logger::error("Main", "Operation failed: {}", e.what());
}
```
