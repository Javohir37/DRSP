#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "../headers/getDistricts.h"
#include "../headers/localmysql.h"

void minRtrGetDistricts(const char *json_request, char *response_buffer, size_t buffer_size) {
    // Parse the JSON request
    MYSQL *conn = establish_connection();
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 1) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments for 'getDistricts'\"}");
        json_object_put(parsed_json);
        return;
    }

    const char *regionName = json_object_get_string(json_object_array_get_idx(args_obj, 0));

    // Call the getDistricts logic
    char *result = getDistricts(conn, regionName);
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to fetch districts\"}");
    }

    json_object_put(parsed_json);
}

