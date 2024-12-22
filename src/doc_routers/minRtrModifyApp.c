#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "modifyApp.h"
#include "localmysql.h"
#include "minRtrModifyApp.h"

void minRtrModifyApp(const char *json_request, char *response_buffer, size_t buffer_size) {
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

    // Extract arguments
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) ||
        json_object_array_length(args_obj) < 2) {
        snprintf(response_buffer, buffer_size,
                 "{\"error\": \"Missing or invalid arguments. Required: appointmentID, status\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Get the arguments
    int appointmentID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *status = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    // Validate the parameters
    if (appointmentID <= 0 || !status) {
        snprintf(response_buffer, buffer_size,
                 "{\"error\": \"Invalid arguments. appointmentID must be positive and status must be non-null\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Call modifyApp function
    const char *result = modifyApp(conn, appointmentID, status);
    snprintf(response_buffer, buffer_size, "{\"message\": \"%s\"}", result);

    // Clean up
    json_object_put(parsed_json);
    mysql_close(conn);
}

