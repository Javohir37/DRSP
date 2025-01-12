#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/modifAppoint.h"
#include "../headers/localmysql.h"

void minRtrModifAppoint(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    // Parse the input JSON request
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

    // Extract arguments from 'args'
    struct json_object *patientID_obj = json_object_array_get_idx(args_obj, 0);
    struct json_object *appointmentID_obj = json_object_array_get_idx(args_obj, 1);
    struct json_object *status_obj = json_object_array_get_idx(args_obj, 2);

    if (!patientID_obj || !appointmentID_obj || !status_obj) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid or missing arguments in 'args'\"}");
        json_object_put(parsed_json);
        return;
    }

    if (!json_object_is_type(patientID_obj, json_type_int) ||
        !json_object_is_type(appointmentID_obj, json_type_int) ||
        !json_object_is_type(status_obj, json_type_string)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid data types in 'args'\"}");
        json_object_put(parsed_json);
        return;
    }

    int patientID = json_object_get_int(patientID_obj);
    int appointmentID = json_object_get_int(appointmentID_obj);
    const char *status = json_object_get_string(status_obj);

    // Call modifAppoint and get the result
    char *result_json = modifAppoint(conn, patientID, appointmentID, status);
    if (!result_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to update appointment\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result_json);
        free(result_json);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

