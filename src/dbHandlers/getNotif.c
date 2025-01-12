#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>

char* getNotif(MYSQL *conn, int patientID, const char *lastLoginDate) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    struct json_object *response = json_object_new_array();

    // Query for active appointments
    char query[1024];
    snprintf(query, sizeof(query), 
             "SELECT a.AppointmentID, a.DateTime, a.Status, d.FullName, d.Spec, h.Name AS HospitalName "
             "FROM appointments a "
             "JOIN doctors d ON a.DoctorID = d.DoctorID "
             "JOIN hospitals h ON d.HospitalID = h.HospitalID "
             "WHERE a.PatientID = %d AND a.ModifiedDateTime > '%s';",
             patientID, lastLoginDate);

    if (mysql_query(conn, query)) {
        json_object_put(response);
        return strdup("{\"error\":\"Database query failed\"}");
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        json_object_put(response);
        return strdup("{\"error\":\"Failed to retrieve query results\"}");
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        struct json_object *appointment = json_object_new_object();
        json_object_object_add(appointment, "AppointmentID", json_object_new_int(atoi(row[0])));
        json_object_object_add(appointment, "DateTime", json_object_new_string(row[1]));
        json_object_object_add(appointment, "Status", json_object_new_string(row[2]));
        json_object_object_add(appointment, "DoctorName", json_object_new_string(row[3]));
        json_object_object_add(appointment, "Specialization", json_object_new_string(row[4]));
        json_object_object_add(appointment, "HospitalName", json_object_new_string(row[5]));
        json_object_array_add(response, appointment);
    }
    mysql_free_result(result);

    // Query for active waitlist items
    snprintf(query, sizeof(query), 
             "SELECT w.WaitlistID, w.Date, w.Status, d.FullName, d.Spec, h.Name AS HospitalName "
             "FROM waitlist w "
             "JOIN doctors d ON w.DoctorID = d.DoctorID "
             "JOIN hospitals h ON d.HospitalID = h.HospitalID "
             "WHERE w.PatientID = %d AND w.ModifiedDateTime > '%s';",
             patientID, lastLoginDate);

    if (mysql_query(conn, query)) {
        json_object_put(response);
        return strdup("{\"error\":\"Database query failed\"}");
    }

    result = mysql_store_result(conn);
    if (result == NULL) {
        json_object_put(response);
        return strdup("{\"error\":\"Failed to retrieve query results\"}");
    }

    while ((row = mysql_fetch_row(result))) {
        struct json_object *waitlist = json_object_new_object();
        json_object_object_add(waitlist, "WaitlistID", json_object_new_int(atoi(row[0])));
        json_object_object_add(waitlist, "Date", json_object_new_string(row[1]));
        json_object_object_add(waitlist, "Status", json_object_new_string(row[2]));
        json_object_object_add(waitlist, "DoctorName", json_object_new_string(row[3]));
        json_object_object_add(waitlist, "Specialization", json_object_new_string(row[4]));
        json_object_object_add(waitlist, "HospitalName", json_object_new_string(row[5]));
        json_object_array_add(response, waitlist);
    }
    mysql_free_result(result);

    char *response_str = strdup(json_object_to_json_string(response));
    json_object_put(response);

    return response_str;
}

