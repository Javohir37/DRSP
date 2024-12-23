// minRtrPatientLogin.c

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
    struct json_object *patientID_json;
    struct json_object *password_json;

    parsed_json = json_tokener_parse(requestJSON);

    if (!parsed_json) {
        return strdup("{\"error\":\"Invalid JSON format\"}");
    }

    if (!json_object_object_get_ex(parsed_json, "PatientID", &patientID_json) ||
        !json_object_object_get_ex(parsed_json, "Password", &password_json)) {
        json_object_put(parsed_json);
        return strdup("{\"error\":\"Invalid input JSON\"}");
    }

    int patientID = json_object_get_int(patientID_json);
    const char *password = json_object_get_string(password_json);

    // Call the patientLogin function
    char *response = patientLogin(conn, patientID, password);

    // Clean up parsed JSON
    json_object_put(parsed_json);

    return response;
}

// minRtrPatientLogin.h

#ifndef MIN_RTR_PATIENT_LOGIN_H
#define MIN_RTR_PATIENT_LOGIN_H

#include <mysql/mysql.h>

// Function to handle patient login request and generate a response
char* handlePatientLoginRequest(MYSQL *conn, const char *requestJSON);

#endif // MIN_RTR_PATIENT_LOGIN_H

