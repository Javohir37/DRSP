#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "modifAppoint.h"
#include "minRtrModifAppoint.h"

char* handleModifAppointRequest(MYSQL *conn, const char *requestJSON) {
    struct json_object *parsed_json;
    struct json_object *args_obj;

    parsed_json = json_tokener_parse(requestJSON);
    if (!parsed_json) {
        return strdup("{\"error\":\"Invalid JSON format\"}");
    }

    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 3) {
        json_object_put(parsed_json);
        return strdup("{\"error\":\"Missing or invalid arguments. Required: PatientID, AppointmentID, and Status\"}");
    }

    // Extract PatientID, AppointmentID, and Status from args array
    int patientID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    int appointmentID = json_object_get_int(json_object_array_get_idx(args_obj, 1));
    const char *status = json_object_get_string(json_object_array_get_idx(args_obj, 2));

    // Call the modifAppoint function
    char *response = modifAppoint(conn, patientID, appointmentID, status);

    json_object_put(parsed_json);

    return response;
}

