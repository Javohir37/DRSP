#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "../headers/selectHospital.h"
#include "../headers/localmysql.h"
#include "../headers/minRtrSelectHospital.h"

void minRtrSelectHospital(const char *json_request, char *response_buffer, size_t buffer_size) {
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
        json_object_array_length(args_obj) < 3) {
        snprintf(response_buffer, buffer_size, 
                "{\"error\": \"Missing or invalid arguments. Required: name, region, district\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Get the arguments
    const char *name = json_object_get_string(json_object_array_get_idx(args_obj, 0));
    const char *region = json_object_get_string(json_object_array_get_idx(args_obj, 1));
    const char *district = json_object_get_string(json_object_array_get_idx(args_obj, 2));


    // Validate all parameters are present
    if (!name || !region || !district ) {
        snprintf(response_buffer, buffer_size, 
                "{\"error\": \"All parameters (name, region, district) must be non-null strings\"}");
        json_object_put(parsed_json);
        mysql_close(conn);
        return;
    }

    // Call selectHospital function
    char *result = selectHospital(conn, name, region, district);

    snprintf(response_buffer, buffer_size, "%s", result);
	
    // Clean up
    json_object_put(parsed_json);
    mysql_close(conn);
}
