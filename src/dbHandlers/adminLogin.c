#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>

char* adminLogin(MYSQL *conn, int adminID, const char *password) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    // Prepare the SQL query
    char query[512];
    snprintf(query, sizeof(query),
             "SELECT AdminID FROM admins WHERE AdminID = %d AND Password = '%s'",
             adminID, password);

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
        json_object_object_add(response, "AdminID", json_object_new_int(atoi(row[0])));

        mysql_free_result(result);
        return strdup(json_object_to_json_string(response));
    }

    // Return "false" for invalid credentials
    mysql_free_result(result);
    return strdup("{\"result\":\"false\"}");
}

