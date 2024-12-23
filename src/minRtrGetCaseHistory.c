// minRtrGetCaseHistory.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "getCaseHistory.h"
#include "minRtrGetCaseHistory.h"

char* handleGetCaseHistoryRequest(MYSQL *conn, const char *requestJSON) {
    struct json_object *parsed_json;
    struct json_object *patientID_json;

    parsed_json = json_tokener_parse(requestJSON);
    if (!parsed_json) {
        return strdup("{\"error\":\"Invalid JSON format\"}");
    }

    if (!json_object_object_get_ex(parsed_json, "PatientID", &patientID_json)) {
        json_object_put(parsed_json);
        return strdup("{\"error\":\"Invalid input JSON\"}");
    }

    int patientID = json_object_get_int(patientID_json);
    char *response = getCaseHistory(conn, patientID);

    json_object_put(parsed_json);

    return response;
}

