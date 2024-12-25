// minRtrGetSpecs.c
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "../headers/getSpecs.h"
#include "../headers/localmysql.h"

void minRtrGetSpecs(const char *json_request, char *response_buffer, size_t buffer_size) {
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
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 1) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments. Required: hospitalID\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Get the hospitalID argument
    int hospitalID = json_object_get_int(json_object_array_get_idx(args_obj, 0));

    // Call the getSpecs function
    char *result = getSpecs(conn, hospitalID);
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve specializations\"}");
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

