#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "getSpecs.h"

// Include the jsonParser header
#include "jsonParser.h"

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
    char *json = generate_json(all_rows, fields, num_rows, num_fields);

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
