#include <stdio.h>
#include <string.h>
#include <stddef.h> // Include for size_t
#include <json-c/json.h>
#include "../headers/getSchedule.h"
#include "../headers/localmysql.h"

void minRtrGetSchedule(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    // Parse the input JSON
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || !json_object_is_type(args_obj, json_type_array)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid or missing 'args' array\"}");
        json_object_put(parsed_json);
        return;
    }

    // Extract positional arguments from 'args'
    struct json_object *date_obj = json_object_array_get_idx(args_obj, 0);
    struct json_object *doctorID_obj = json_object_array_get_idx(args_obj, 1);

    if (!date_obj || !doctorID_obj) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid or missing arguments in 'args'\"}");
        json_object_put(parsed_json);
        return;
    }

    if (!json_object_is_type(date_obj, json_type_string) || !json_object_is_type(doctorID_obj, json_type_string)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid data types in 'args'\"}");
        json_object_put(parsed_json);
        return;
    }

    const char *date = json_object_get_string(date_obj);
    const char *doctorID = json_object_get_string(doctorID_obj);

    printf("DEBUG: date = %s, doctorID = %s\n", date, doctorID);

    // Call getSchedule function
    char *result_json = getSchedule(conn, date, doctorID);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve schedule\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result_json);
        free(result_json);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

