#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "jsonParser.h"
#include <string.h>


int main(int argc, char *argv[]) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_FIELD *fields;

    // Database credentials
    const char *HOST = "localhost";
    const char *USER = "root";
    const char *PASS = "";
    const char *DB_NAME = "DRSP";

    // Connect to MySQL database
    conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "mysql_init() failed\n");
        return EXIT_FAILURE;
    }

    if (mysql_real_connect(conn, HOST, USER, PASS, DB_NAME, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Execute the query
    const char *query = "SELECT DISTINCT Region FROM HOSPITALS";
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT DISTINCT Region failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Retrieve and store result
    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    int row_count = mysql_num_rows(res);
    if (row_count == 0) {
        fprintf(stderr, "No regions found.\n");
        mysql_free_result(res);
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    int num_fields = mysql_num_fields(res);
    fields = mysql_fetch_fields(res);

    // Allocate memory for rows
    char ***all_rows = malloc(row_count * sizeof(char **));
    if (!all_rows) {
        fprintf(stderr, "Memory allocation failed for all_rows\n");
        mysql_free_result(res);
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Populate rows
    for (int i = 0; i < row_count; i++) {
        row = mysql_fetch_row(res);
        all_rows[i] = malloc(num_fields * sizeof(char *));
        for (int j = 0; j < num_fields; j++) {
            all_rows[i][j] = row[j] ? strdup(row[j]) : NULL;
        }
    }

    // Generate JSON
    char *json = generate_json(all_rows, fields, row_count, num_fields);
    printf("%s\n", json);

    // Free memory
    free(json);
    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < num_fields; j++) {
            free(all_rows[i][j]);
        }
        free(all_rows[i]);
    }
    free(all_rows);

    mysql_free_result(res);
    mysql_close(conn);

    return EXIT_SUCCESS;
}

