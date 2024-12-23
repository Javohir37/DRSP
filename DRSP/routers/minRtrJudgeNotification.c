#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/judgeNotification.h"
#include "../headers/localmysql.h"

void minRtrJudgeNotification(MYSQL *conn, const char *json_request, char *response_buffer, size_t buffer_size) {
    printf("DEBUG: Inside minRtrJudgeNotification\n");

    // Parse the input JSON
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    // Extract arguments
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 3) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments for 'judgeNotification'\"}");
        json_object_put(parsed_json);
        return;
    }

    // Get arguments: ID, Status, TableName
    int id = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *status = json_object_get_string(json_object_array_get_idx(args_obj, 1));
    const char *tableName = json_object_get_string(json_object_array_get_idx(args_obj, 2));

    printf("DEBUG: ID: %d, Status: %s, TableName: %s\n", id, status, tableName);

    // Call judgeNotification and handle the result
    char *result = judgeNotification(conn, id, status, tableName);
    if (result) {
        snprintf(response_buffer, buffer_size, "{\"message\": \"%s\"}", result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to process notification\"}");
    }

    json_object_put(parsed_json);
}

