// minRtrGetDocNotification.c

#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/getDocNotification.h"
#include "../headers/localmysql.h"

void minRtrGetDocNotification(const char *json_request, char *response_buffer, size_t buffer_size) {
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

    struct json_object *doctorID_obj, *lastLogin_obj;
    if (!json_object_object_get_ex(parsed_json, "doctorID", &doctorID_obj) ||
        !json_object_object_get_ex(parsed_json, "lastLogin", &lastLogin_obj)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing required fields\"}");
        json_object_put(parsed_json);
        return;
    }

    int doctorID = json_object_get_int(doctorID_obj);
    const char *lastLogin = json_object_get_string(lastLogin_obj);

    // Call getDocNotification function
    char *result_json = getDocNotification(conn, doctorID, lastLogin);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve notifications\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result_json);
        free(result_json);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

