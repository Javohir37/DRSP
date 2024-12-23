// getCaseHistory.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "getCaseHistory.h"

char* getCaseHistory(MYSQL *conn, int patientID) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    char query[1024];
    snprintf(query, sizeof(query),
             "SELECT CaseID, PatientID, DoctorID, VisitDate, Prescription, Tests, Diagnosis, Notes "
             "FROM casehistory WHERE PatientID = %d", patientID);

    if (mysql_query(conn, query)) {
        return strdup("{\"error\":\"Database query failed\"}");
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        return strdup("{\"error\":\"Failed to retrieve query results\"}");
    }

    struct json_object *response = json_object_new_array();
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        struct json_object *case_entry = json_object_new_object();
        json_object_object_add(case_entry, "CaseID", json_object_new_int(atoi(row[0])));
        json_object_object_add(case_entry, "PatientID", json_object_new_int(atoi(row[1])));
        json_object_object_add(case_entry, "DoctorID", json_object_new_int(atoi(row[2])));
        json_object_object_add(case_entry, "VisitDate", json_object_new_string(row[3]));
        json_object_object_add(case_entry, "Prescription", json_object_new_string(row[4]));
        json_object_object_add(case_entry, "Tests", json_object_new_string(row[5]));
        json_object_object_add(case_entry, "Diagnosis", json_object_new_string(row[6]));
        json_object_object_add(case_entry, "Notes", json_object_new_string(row[7]));

        json_object_array_add(response, case_entry);
    }

    mysql_free_result(result);
    return strdup(json_object_to_json_string(response));
}

