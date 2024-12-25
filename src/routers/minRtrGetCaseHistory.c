#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include <stddef.h>  // Include this for size_t
#include "../headers/getCaseHistory.h"
#include "../headers/localmysql.h"

void minRtrGetCaseHistory(const char *json_request, char *response_buffer, size_t buffer_size) {
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        return;
    }

    // Parse the JSON request
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

    // Extract the first argument (PatientID) from 'args' as a string
    struct json_object *patient_id_obj = json_object_array_get_idx(args_obj, 0);
    if (!patient_id_obj || !json_object_is_type(patient_id_obj, json_type_string)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid or missing PatientID in 'args'\"}");
        json_object_put(parsed_json);
        return;
    }

    const char *patient_id_str = json_object_get_string(patient_id_obj);
    printf("DEBUG: Extracted PatientID = %s\n", patient_id_str);

    // Convert PatientID to an integer
    int patient_id = atoi(patient_id_str);

    // Call the getCaseHistory function
    char *result = getCaseHistory(conn, patient_id);
    if (!result) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve case history\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result);
    }

    json_object_put(parsed_json);
    mysql_close(conn);
}

