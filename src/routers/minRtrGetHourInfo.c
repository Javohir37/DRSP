#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/getHourInfo.h"
#include "../headers/localmysql.h"

void minRtrGetHourInfo(MYSQL *conn, const char *json_request, char *response_buffer, size_t buffer_size) {
    printf("DEBUG: Inside minRtrGetHourInfo\n");

    // Parse the input JSON
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    // Extract arguments
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 2) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments for 'getHourInfo'\"}");
        json_object_put(parsed_json);
        return;
    }

    // Get arguments: DoctorID and DateTime
    int doctorID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *dateTime = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    printf("DEBUG: DoctorID: %d, DateTime: %s\n", doctorID, dateTime);

    // Call getHourInfo and handle the result
    char *result = getHourInfo(conn, doctorID, dateTime);
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result); // Free the dynamically allocated JSON string
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve hourly info\"}");
    }

    json_object_put(parsed_json);
}

