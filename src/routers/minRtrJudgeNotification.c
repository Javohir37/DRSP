#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/judgeNotification.h"
#include "../headers/localmysql.h"

// Mini-router for the `judgeNotification` function
void minRtrJudgeNotification(const char *json_request, char *response_buffer, size_t buffer_size) {
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
    struct json_object *id_obj = json_object_array_get_idx(args_obj, 0);
    struct json_object *status_obj = json_object_array_get_idx(args_obj, 1);
    struct json_object *table_obj = json_object_array_get_idx(args_obj, 2);

    if (!id_obj || !status_obj || !table_obj) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid or missing arguments in 'args'\"}");
        json_object_put(parsed_json);
        return;
    }

    if (!json_object_is_type(id_obj, json_type_string) || 
        !json_object_is_type(status_obj, json_type_string) || 
        !json_object_is_type(table_obj, json_type_string)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid data types in 'args'\"}");
        json_object_put(parsed_json);
        return;
    }

    const char *id = json_object_get_string(id_obj);
    const char *status = json_object_get_string(status_obj);
    const char *tableName = json_object_get_string(table_obj);

    printf("DEBUG: id = %s, status = %s, tableName = %s\n", id, status, tableName);

    // Call judgeNotification function
    char *result_json = judgeNotification(conn, id, status, tableName);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to process judgeNotification\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result_json);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

