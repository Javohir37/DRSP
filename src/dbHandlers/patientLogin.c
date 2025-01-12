#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "../headers/patientLogin.h"

char* patientLogin(MYSQL *conn, const char *patientID, const char *password) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    // Prepare escaped strings to prevent SQL injection
    char escapedPatientID[256];
    char escapedPassword[256];
    mysql_real_escape_string(conn, escapedPatientID, patientID, strlen(patientID));
    mysql_real_escape_string(conn, escapedPassword, password, strlen(password));

    // Prepare the SQL query with sufficient buffer size
    char query[1024];
    snprintf(query, sizeof(query),
             "SELECT PatientID, Name, LastLoginDate FROM patients WHERE PatientID = '%s' AND Password = '%s'",
             escapedPatientID, escapedPassword);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return strdup("{\"error\":\"Database query failed\"}");
    }

    // Retrieve the result
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return strdup("{\"error\":\"Failed to retrieve query results\"}");
    }

    MYSQL_ROW row;
    struct json_object *response;

    // Check if any row is returned
    if ((row = mysql_fetch_row(result))) {
        // Create JSON response for valid login
        response = json_object_new_object();
        json_object_object_add(response, "PatientID", json_object_new_string(row[0]));
        json_object_object_add(response, "Name", json_object_new_string(row[1]));
        if (row[2]) { // Check if LastLoginDate is not NULL
            json_object_object_add(response, "LastLogin", json_object_new_string(row[2]));
        } else {
            json_object_object_add(response, "LastLogin", json_object_new_string("Never"));
        }

        // Update the LastLoginDate
        snprintf(query, sizeof(query),
                 "UPDATE patients SET LastLoginDate = NOW() WHERE PatientID = '%s'",
                 escapedPatientID);
        mysql_query(conn, query);

        mysql_free_result(result);
        return strdup(json_object_to_json_string(response));
    }

    // Return error for invalid credentials or non-existent patient
    response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("Invalid credentials"));
    mysql_free_result(result);
    return strdup(json_object_to_json_string(response));
}

