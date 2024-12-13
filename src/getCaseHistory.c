#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "getCaseHistory.h"

// Function to get the case history for a given PatientID and return the case history in a JSON-like string
char* getCaseHistory(MYSQL *conn, int patientID) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[1024];
    static char result[4096];  // Static buffer to hold the result as a string

    // Construct the SELECT query to find case history based on PatientID and join with the doctors table
    snprintf(query, sizeof(query),
             "SELECT ch.CaseID, ch.VisitDate, ch.Prescription, ch.Tests, ch.Diagnosis, ch.Notes, "
             "d.FullName, d.Spec, d.Role "
             "FROM casehistory ch "
             "INNER JOIN doctors d ON ch.DoctorID = d.DoctorID "
             "WHERE ch.PatientID = %d", 
             patientID);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        return "we did not get your output"; // Return default error message
    }

    // Retrieve result
    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return "we did not get your output"; // Return default error message
    }

    // If no row is found, return the default message
    if (mysql_num_rows(res) == 0) {
        mysql_free_result(res);
        return "No case history found for this patient.";
    }

    // Initialize result as an empty string
    result[0] = '\0';

    // Iterate over the result set and append each row to the result string
    while ((row = mysql_fetch_row(res)) != NULL) {
        int caseID = atoi(row[0]);  // Get the CaseID
        const char *visitDate = row[1];  // Get the VisitDate
        const char *prescription = row[2];  // Get the Prescription
        const char *tests = row[3];  // Get the Tests
        const char *diagnosis = row[4];  // Get the Diagnosis
        const char *notes = row[5];  // Get the Notes
        const char *fullName = row[6];  // Get the FullName of the doctor
        const char *spec = row[7];  // Get the Spec (specialization) of the doctor
        const char *role = row[8];  // Get the Role of the doctor

        // Append the case information in a JSON-like format
        snprintf(result + strlen(result), sizeof(result) - strlen(result),
                 "{\"CaseID\": %d, \"VisitDate\": \"%s\", \"Prescription\": \"%s\", \"Tests\": \"%s\", \"Diagnosis\": \"%s\", \"Notes\": \"%s\", "
                 "\"Doctor\": {\"FullName\": \"%s\", \"Spec\": \"%s\", \"Role\": \"%s\"}}, ",
                 caseID, visitDate, prescription, tests, diagnosis, notes, fullName, spec, role);
    }

    // Remove the last comma and space
    result[strlen(result) - 2] = '\0';

    mysql_free_result(res);

    return result; // Return the result as a string
}

