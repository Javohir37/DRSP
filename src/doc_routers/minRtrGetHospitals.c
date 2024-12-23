// minRtrGetHospitals.c

#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "getHospitals.h"
#include "localmysql.h"

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

    struct json_object *region_obj, *district_obj;
    if (!json_object_object_get_ex(parsed_json, "region", &region_obj) ||
        !json_object_object_get_ex(parsed_json, "district", &district_obj)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing required fields\"}");
        json_object_put(parsed_json);
        return;
    }

    const char *region = json_object_get_string(region_obj);
    const char *district = json_object_get_string(district_obj);

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

