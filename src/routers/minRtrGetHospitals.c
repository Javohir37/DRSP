#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/getHospitals.h"
#include "../headers/localmysql.h"

void minRtrGetHospitals(const char *json_request, char *response_buffer, size_t buffer_size) {
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
    const char *region = json_object_get_string(json_object_array_get_idx(args_obj, 0));
    const char *district = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    if (!region || !district) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid arguments\"}");
        json_object_put(parsed_json);
        return;
    }

    // Call getHospitals function
    char *result_json = getHospitals(conn, region, district);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve hospitals\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result_json);
        free(result_json);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}
