// minRtrGetSchedule.c

#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "getSchedule.h"
#include "localmysql.h"

void minRtrGetSchedule(const char *json_request, char *response_buffer, size_t buffer_size) {
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

    struct json_object *date_obj, *doctorID_obj;
    if (!json_object_object_get_ex(parsed_json, "date", &date_obj) ||
        !json_object_object_get_ex(parsed_json, "doctorID", &doctorID_obj)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing required fields\"}");
        json_object_put(parsed_json);
        return;
    }

    const char *date = json_object_get_string(date_obj);
    int doctorID = json_object_get_int(doctorID_obj);

    // Call getSchedule function
    char *result_json = getSchedule(conn, date, doctorID);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve schedule\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result_json);
        free(result_json);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

