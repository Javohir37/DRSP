#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "patientLogin.h"

char* patientLogin(MYSQL *conn, int patientID, const char *password) {
    char query[512];
   snprintf(query, sizeof(query),
         "SELECT Name, DOB, LastLoginDate FROM patients WHERE PatientID=%d AND Password='%s'",
         patientID, password);


    if (mysql_query(conn, query)) {
        fprintf(stderr, "SQL query failed: %s\n", mysql_error(conn));
        char *error_response = malloc(64);
        snprintf(error_response, 64, "{\"error\":\"Database query failed\"}");
        return error_response;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result) {
        fprintf(stderr, "SQL store result failed: %s\n", mysql_error(conn));
        char *error_response = malloc(64);
        snprintf(error_response, 64, "{\"error\":\"Database query failed\"}");
        return error_response;
    }

    if (mysql_num_rows(result) == 0) {
        mysql_free_result(result);
        char *error_response = malloc(64);
        snprintf(error_response, 64, "{\"error\":\"Invalid credentials\"}");
        return error_response;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    char *response = malloc(256);
    snprintf(response, 256,
             "{\"PatientID\":%d,\"Name\":\"%s\",\"DOB\":\"%s\",\"LastLoginDate\":\"%s\"}",
             patientID, row[0], row[1], row[2] ? row[2] : "NULL");

    mysql_free_result(result);
    return response;
}

