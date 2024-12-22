#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "getCaseHistory.h"

void minRtrGetCaseHistory(MYSQL *conn, const char *json_request, char *response_buffer, size_t buffer_size) {
    printf("DEBUG: Inside minRtrGetCaseHistory\n");

    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 1) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments for 'getCaseHistory'\"}");
        json_object_put(parsed_json);
        return;
    }

    int patientID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    printf("DEBUG: PatientID: %d\n", patientID);

    char *result = getCaseHistory(conn, patientID);
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve case history\"}");
    }

    json_object_put(parsed_json);
}

