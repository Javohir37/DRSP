#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

// Function to retrieve regions from the database
char ***get_regions(MYSQL *conn, MYSQL_FIELD **fields, int *row_count, int *num_fields) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    // Execute query
    const char *query = "SELECT DISTINCT Region FROM hospitals";
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return NULL;
    }

    // Retrieve result
    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return NULL;
    }

    *row_count = mysql_num_rows(res);
    *num_fields = mysql_num_fields(res);

    if (*row_count == 0) {
        fprintf(stderr, "No regions found.\n");
        mysql_free_result(res);
        return NULL;
    }

    *fields = mysql_fetch_fields(res);

    // Allocate memory for rows
    char ***all_rows = malloc(*row_count * sizeof(char **));
    if (!all_rows) {
        fprintf(stderr, "Memory allocation failed for all_rows\n");
        mysql_free_result(res);
        return NULL;
    }

    // Populate rows with data
    for (int i = 0; i < *row_count; i++) {
        row = mysql_fetch_row(res);
        all_rows[i] = malloc(*num_fields * sizeof(char *));
        if (!all_rows[i]) {
            fprintf(stderr, "Memory allocation failed for row %d\n", i);
            // Free already allocated rows
            for (int j = 0; j < i; j++) {
                for (int k = 0; k < *num_fields; k++) {
                    free(all_rows[j][k]);
                }
                free(all_rows[j]);
            }
            free(all_rows);
            mysql_free_result(res);
            return NULL;
        }

        for (int j = 0; j < *num_fields; j++) {
            all_rows[i][j] = row[j] ? strdup(row[j]) : NULL;
        }
    }

    mysql_free_result(res);
    return all_rows;
}

// Function to free allocated memory for rows
void free_regions(char ***all_rows, int row_count, int num_fields) {
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < num_fields; j++) {
            free(all_rows[i][j]);
        }
        free(all_rows[i]);
    }
    free(all_rows);
}

