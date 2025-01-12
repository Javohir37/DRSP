#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>

/**
 * Function to retrieve IP addresses and port numbers from the database based on status.
 *
 * @param conn The MySQL connection object.
 * @param status The status to filter IP addresses.
 * @return A JSON-formatted string with IP addresses and port numbers.
 */
char* getIP(MYSQL *conn, const char *status) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    // Query to fetch IPs with the given status
    char query[256];
    snprintf(query, sizeof(query), "SELECT IP, PortNum FROM ip WHERE Status = '%s'", status);

    if (mysql_query(conn, query)) {
        return strdup("{\"error\":\"Database query failed\"}");
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        return strdup("{\"error\":\"Failed to retrieve query results\"}");
    }

    MYSQL_ROW row;
    struct json_object *response_array = json_object_new_array();

    // Iterate over rows and add them to the JSON response
    while ((row = mysql_fetch_row(result))) {
        struct json_object *entry = json_object_new_object();
        json_object_object_add(entry, "IP", json_object_new_string(row[0]));
        json_object_object_add(entry, "Port", json_object_new_string(row[1]));
        json_object_array_add(response_array, entry);
    }

    mysql_free_result(result);

    // Return the JSON response as a string
    return strdup(json_object_to_json_string(response_array));
}
