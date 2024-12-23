// minRtrModifAppoint.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "modifAppoint.h"
#include "minRtrModifAppoint.h"

char* handleModifAppointRequest(MYSQL *conn, const char *requestJSON) {
    struct json_object *parsed_json;
    struct json_object *patientID_json;
    struct json_object *appointmentID_json;
    struct json_object *status_json;

    parsed_json = json_tokener_parse(requestJSON);
    if (!parsed_json) {
        return strdup("{\"error\":\"Invalid JSON format\"}");
    }

    if (!json_object_object_get_ex(parsed_json, "PatientID", &patientID_json) ||
        !json_object_object_get_ex(parsed_json, "AppointmentID", &appointmentID_json) ||
        !json_object_object_get_ex(parsed_json, "Status", &status_json)) {
        json_object_put(parsed_json);
        return strdup("{\"error\":\"Invalid input JSON\"}");
    }

    int patientID = json_object_get_int(patientID_json);
    int appointmentID = json_object_get_int(appointmentID_json);
    const char *status = json_object_get_string(status_json);

    char *response = modifAppoint(conn, patientID, appointmentID, status);

    json_object_put(parsed_json);

    return response;
}

