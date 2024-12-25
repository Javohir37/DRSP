#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include "../headers/getCaseHistory.h"
#include <json-c/json.h>

// Function to get the case history for a given PatientID and return the case history in a JSON string
char* getCaseHistory(MYSQL *conn, int patientID) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[1024];

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
        return strdup("{\"error\":\"Query execution failed\"}");
    }

    // Retrieve result
    res = mysql_store_result(conn);
    if (!res) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return strdup("{\"error\":\"Failed to store result\"}");
    }

    // If no rows are found, return an empty JSON array
    if (mysql_num_rows(res) == 0) {
        mysql_free_result(res);
        return strdup("[]");
    }

    // Create a JSON array to hold the case history
    struct json_object *json_array = json_object_new_array();

    // Iterate over the result set and add each row to the JSON array
    while ((row = mysql_fetch_row(res)) != NULL) {
        struct json_object *case_obj = json_object_new_object();
        struct json_object *doctor_obj = json_object_new_object();

        // Add case details to the JSON object
        json_object_object_add(case_obj, "CaseID", json_object_new_int(atoi(row[0])));
        json_object_object_add(case_obj, "VisitDate", json_object_new_string(row[1]));
        json_object_object_add(case_obj, "Prescription", json_object_new_string(row[2]));
        json_object_object_add(case_obj, "Tests", json_object_new_string(row[3]));
        json_object_object_add(case_obj, "Diagnosis", json_object_new_string(row[4]));
        json_object_object_add(case_obj, "Notes", json_object_new_string(row[5]));

        // Add doctor details to the JSON object
        json_object_object_add(doctor_obj, "FullName", json_object_new_string(row[6]));
        json_object_object_add(doctor_obj, "Spec", json_object_new_string(row[7]));
        json_object_object_add(doctor_obj, "Role", json_object_new_string(row[8]));

        // Add the doctor object to the case object
        json_object_object_add(case_obj, "Doctor", doctor_obj);

        // Add the case object to the JSON array
        json_object_array_add(json_array, case_obj);
    }

    // Convert the JSON array to a string
    const char *json_result = json_object_to_json_string(json_array);

    // Copy the result to a dynamically allocated string
    char *result = strdup(json_result);

    // Free JSON object and MySQL result
    json_object_put(json_array);
    mysql_free_result(res);

    return result; // Return the JSON string
}

