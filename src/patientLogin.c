#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>

char* patientLogin(MYSQL *conn, int patientID, const char *password) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    // Prepare the SQL query
    char query[512];
    snprintf(query, sizeof(query),
             "SELECT PatientID, Name, LastLoginDate FROM patients WHERE PatientID = %d AND Password = '%s'",
             patientID, password);

    if (mysql_query(conn, query)) {
        return strdup("{\"error\":\"Database query failed\"}");
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        return strdup("{\"error\":\"Failed to retrieve query results\"}");
    }

    MYSQL_ROW row;
    struct json_object *response;

    // Check if any row is returned
    if ((row = mysql_fetch_row(result))) {
        // Create JSON response for valid login
        response = json_object_new_object();
        json_object_object_add(response, "PatientID", json_object_new_int(atoi(row[0])));
        json_object_object_add(response, "Name", json_object_new_string(row[1]));
        if (row[2]) { // Check if LastLoginDate is not NULL
            json_object_object_add(response, "LastLogin", json_object_new_string(row[2]));
        } else {
            json_object_object_add(response, "LastLogin", json_object_new_string("Never"));
        }

        // Update the LastLoginDate
        snprintf(query, sizeof(query),
                 "UPDATE patients SET LastLoginDate = NOW() WHERE PatientID = %d", patientID);
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
