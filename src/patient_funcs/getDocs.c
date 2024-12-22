#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Add this for strdup
#include <mysql/mysql.h>
#include <json-c/json.h>

char* getDocs(MYSQL *conn, int hospitalID, const char *spec) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    // Prepare the SQL query
    char query[512];
    snprintf(query, sizeof(query),
             "SELECT FullName FROM doctors WHERE HospitalID = %d AND Spec = '%s'",
             hospitalID, spec);

    if (mysql_query(conn, query)) {
        return strdup("{\"error\":\"Database query failed\"}");
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        return strdup("{\"error\":\"Failed to retrieve query results\"}");
    }

    MYSQL_ROW row;
    struct json_object *response = json_object_new_array();

    // Fetch each row and add the doctor's full name to the JSON array
    while ((row = mysql_fetch_row(result))) {
        json_object_array_add(response, json_object_new_string(row[0]));
    }

    mysql_free_result(result);

    // Return the JSON array as a string
    return strdup(json_object_to_json_string(response));
}

