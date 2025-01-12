#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "../headers/selectDoc.h"
#include "../headers/localmysql.h"

void minRtrSelectDoc(const char *json_request, char *response_buffer, size_t buffer_size) {
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
                 "{\"error\": \"Missing or invalid arguments. Required: hospitalID, fullName\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    int hospitalID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *fullName = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    // Call the `selectDoc` function
    char *result = selectDoc(conn, hospitalID, fullName);
    snprintf(response_buffer, buffer_size, "%s", result);
    free(result);

    // Clean up
    json_object_put(parsed_json);
    mysql_close(conn);
}

