#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "adminLogin.h"
#include "localmysql.h"

void minRtrAdminLogin(const char *json_request, char *response_buffer, size_t buffer_size) {
    // Establish database connection
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\":\"Database connection failed\"}");
        return;
    }

    // Parse the JSON request
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\":\"Invalid JSON format\"}");
        mysql_close(conn);
        return;
    }

    // Extract "args" from the JSON request
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj)) {
        snprintf(response_buffer, buffer_size, "{\"error\":\"Missing 'args' field\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Ensure "args" is an array with 2 elements
    if (!json_object_is_type(args_obj, json_type_array) || json_object_array_length(args_obj) != 2) {
        snprintf(response_buffer, buffer_size, "{\"error\":\"Invalid 'args' format, expected [adminID, password]\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Extract adminID and password
    int adminID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *password = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    // Call the `adminLogin` function
    char *result = adminLogin(conn, adminID, password);

    // Copy the result into the response buffer
    snprintf(response_buffer, buffer_size, "%s", result);

    // Free resources
    free(result);
    json_object_put(parsed_json);
    mysql_close(conn);
}

