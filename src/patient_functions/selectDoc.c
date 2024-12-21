#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>

char* selectDoc(MYSQL *conn, int hospitalID, const char *fullName) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    // Prepare the SQL query
    char query[512];
    snprintf(query, sizeof(query),
             "SELECT d.DoctorID, h.Name AS HospitalName, d.Role "
             "FROM doctors d "
             "INNER JOIN hospitals h ON d.HospitalID = h.HospitalID "
             "WHERE d.HospitalID = %d AND d.FullName = '%s'",
             hospitalID, fullName);

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
        // Create JSON response for valid doctor
        response = json_object_new_object();
        json_object_object_add(response, "DoctorID", json_object_new_int(atoi(row[0])));
        json_object_object_add(response, "HospitalName", json_object_new_string(row[1]));
        json_object_object_add(response, "Role", json_object_new_string(row[2]));

        mysql_free_result(result);
        return strdup(json_object_to_json_string(response));
    }

    // Return error for invalid doctor or hospital
    response = json_object_new_object();
    json_object_object_add(response, "error", json_object_new_string("Doctor not found"));
    mysql_free_result(result);
    return strdup(json_object_to_json_string(response));
}

