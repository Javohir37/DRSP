#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <json-c/json.h>
#include "../headers/getSpecs.h"

// Function to generate JSON string from MySQL query results
char* generate_json1(char ***all_rows, MYSQL_FIELD *fields, int num_rows, int num_fields) {
    struct json_object *json_array = json_object_new_array();

    for (int i = 0; i < num_rows; i++) {
        struct json_object *json_object_row = json_object_new_object();
        for (int j = 0; j < num_fields; j++) {
            if (all_rows[i][j]) {
                json_object_object_add(json_object_row, fields[j].name, json_object_new_string(all_rows[i][j]));
            } else {
                json_object_object_add(json_object_row, fields[j].name, NULL);
            }
        }
        json_object_array_add(json_array, json_object_row);
    }

    char *json_string = strdup(json_object_to_json_string(json_array));
    json_object_put(json_array);

    return json_string;
}

// Fetch all distinct specializations from a hospital and return as JSON.
char* getSpecs(MYSQL *conn, int hospitalID) {
    char query[1024];
    snprintf(query, sizeof(query),
             "SELECT DISTINCT Spec FROM doctors WHERE HospitalID = %d", hospitalID);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return NULL;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return NULL;
    }

    int num_fields = mysql_num_fields(result);
    int num_rows = mysql_num_rows(result);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    // Allocate memory for storing all rows
    char ***all_rows = malloc(num_rows * sizeof(char **));
    for (int i = 0; i < num_rows; i++) {
        all_rows[i] = malloc(num_fields * sizeof(char *));
    }

    // Fetch all rows
    int row_index = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            all_rows[row_index][i] = row[i] ? strdup(row[i]) : NULL;
        }
        row_index++;
    }

    // Generate JSON string
    char *json = generate_json1(all_rows, fields, num_rows, num_fields);

    // Free allocated memory
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_fields; j++) {
            if (all_rows[i][j]) {
                free(all_rows[i][j]);
            }
        }
        free(all_rows[i]);
    }
    free(all_rows);

    mysql_free_result(result);

    return json;
}
