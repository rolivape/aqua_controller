/*
 * JSON Logging Utility (json_log)
 *
 * Provides a simple function to format log messages as JSON strings,
 * which is useful for structured logging and easier parsing by external tools.
 */

#ifndef JSON_LOG_H
#define JSON_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Prints a log message formatted as a JSON object to stdout.
 *
 * The JSON object includes a timestamp, the logging tag (component),
 * severity level, and the log message.
 *
 * @param tag The tag of the component generating the log (e.g., "usb_comm_aq").
 * @param severity The severity of the log (e.g., "INFO", "ERROR", "WARN").
 * @param message The log message content.
 */
void log_json_message(const char *tag, const char *severity, const char *message);

#ifdef __cplusplus
}
#endif

#endif // JSON_LOG_H