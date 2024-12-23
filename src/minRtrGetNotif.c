#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "getNotif.h"
#include "localmysql.h"

void minRtrGetNotif(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    // Parse the input JSON
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        mysql_close(conn);
        return;
    }

    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || 
        json_object_array_length(args_obj) < 2) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments. Required: patientID, lastLoginDate\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Extract arguments
    int patientID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *lastLoginDate = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    // Validate arguments
    if (patientID <= 0 || !lastLoginDate) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid arguments\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Call the getNotif function
    char *result = getNotif(conn, patientID, lastLoginDate);
    snprintf(response_buffer, buffer_size, "%s", result);

    // Clean up
    free(result);
    json_object_put(parsed_json);
    mysql_close(conn);
}

