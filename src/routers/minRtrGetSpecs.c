#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/getSpecs.h"
#include "../headers/localmysql.h"

void minRtrGetSpecs(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || 
        json_object_array_length(args_obj) < 1) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments. Required: hospitalID\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    struct json_object *hospitalID_obj = json_object_array_get_idx(args_obj, 0);
    if (!json_object_is_type(hospitalID_obj, json_type_string)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid data type for hospitalID\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    const char *hospitalID = json_object_get_string(hospitalID_obj);

    // Call the getSpecs function
    char *result_json = getSpecs(conn, hospitalID);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve specializations\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result_json);
        free(result_json);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

