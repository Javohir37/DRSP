#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "patientLogin.h"
#include "minRtrPatientLogin.h"

char* handlePatientLoginRequest(MYSQL *conn, const char *requestJSON) {
    // Parse the JSON request
    struct json_object *parsed_json;
    struct json_object *args_obj;

    parsed_json = json_tokener_parse(requestJSON);

    if (!parsed_json) {
        return strdup("{\"error\":\"Invalid JSON format\"}");
    }

    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 2) {
        json_object_put(parsed_json);
        return strdup("{\"error\":\"Missing or invalid arguments. Required: PatientID and Password\"}");
    }

    // Extract the positional arguments (PatientID and Password)
    int patientID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *password = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    // Call the patientLogin function
    char *response = patientLogin(conn, patientID, password);

    // Clean up parsed JSON
    json_object_put(parsed_json);

    return response;
}

