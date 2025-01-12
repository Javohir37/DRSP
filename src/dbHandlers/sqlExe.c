#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/sqlExe.h"
#include "../headers/localmysql.h" // Assuming you have a connection setup here

#define MAX_ROWS 10 // Maximum number of rows to include in JSON

/**
 * Executes a given SQL query and returns the result.
 *
 * @param mysqlQuery The SQL query string to execute.
 * @return sqlResult The result of the SQL query execution.
 */
sqlResult sqlExe(const char *mysqlQuery) {
    sqlResult res;
    res.result = NULL;
    res.status = 0;

    // Establish a connection to the database
    MYSQL *conn = establish_connection(); // Assuming this function is defined elsewhere
    if (conn == NULL) {
        fprintf(stderr, "Database connection failed\n");
        res.status = -1;
        return res;
    }

    // Execute the SQL query without adding LIMIT
    if (mysql_query(conn, mysqlQuery)) {
        fprintf(stderr, "Database query failed: %s\n", mysql_error(conn));
        res.status = -1;
    } else {
        res.result = mysql_store_result(conn);
        if (res.result == NULL && mysql_field_count(conn) > 0) {
            fprintf(stderr, "Failed to retrieve query results: %s\n", mysql_error(conn));
            res.status = -1;
        }
    }

    // Clean up the database connection
    mysql_close(conn);
    return res;
}

/**
 * Converts the SQL result to a JSON string with a limit of MAX_ROWS.
 *
 * @param res The SQL result structure.
 * @return char* The JSON string (caller must free).
 */
char *convertToJson(sqlResult res) {
    if (res.status != 0 || res.result == NULL) {
        return strdup("{\"error\":\"Query execution failed\"}");
    }

    struct json_object *json_array = json_object_new_array();

    MYSQL_ROW row;
    MYSQL_FIELD *fields = mysql_fetch_fields(res.result);
    int num_fields = mysql_num_fields(res.result);
    int row_count = 0;

    while ((row = mysql_fetch_row(res.result)) && row_count < MAX_ROWS) {
        struct json_object *json_row = json_object_new_object();
        for (int i = 0; i < num_fields; i++) {
            if (row[i]) {
                json_object_object_add(json_row, fields[i].name, json_object_new_string(row[i]));
            } else {
                json_object_object_add(json_row, fields[i].name, json_object_new_string(""));
            }
        }
        json_object_array_add(json_array, json_row);
        row_count++;
    }

    char *json_string = strdup(json_object_to_json_string(json_array));
    json_object_put(json_array);
    mysql_free_result(res.result);

    return json_string;
}

