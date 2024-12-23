#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "../headers/getWaitlist.h"
#include "../headers/localmysql.h"
#include "../headers/minRtrGetWaitlist.h"

void minRtrGetWaitlist(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    // Parse the JSON request
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        mysql_close(conn);
        return;
    }

    // Extract arguments array
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || 
        json_object_array_length(args_obj) < 2) {
        snprintf(response_buffer, buffer_size, 
                "{\"error\": \"Missing or invalid arguments. Required: date, doctorID\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Get the arguments
    const char *date = json_object_get_string(json_object_array_get_idx(args_obj, 0));
    int doctorID = json_object_get_int(json_object_array_get_idx(args_obj, 1));

    // Validate parameters
    if (!date) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Date parameter must be a non-null string\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Call getWaitlist function
    char *result = getWaitlist(conn, date, doctorID);
    
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to get waitlist\"}");
    }

    // Clean up
    json_object_put(parsed_json);
    mysql_close(conn);
}
