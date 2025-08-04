/*
 * JSON Logging Utility (json_log)
 */

#include <stdio.h>
#include "esp_log.h"
#include "esp_timer.h"
#include "json_log.h"

void log_json_message(const char *tag, const char *severity, const char *message) {
    // Using printf to ensure the output is clean JSON without the standard ESP-LOG prefix.
    printf("{\"timestamp\": %lld, \"severity\": \"%s\", \"tag\": \"%s\", \"message\": \"%s\"}\n",
           esp_timer_get_time(),
           severity,
           tag,
           message);
}