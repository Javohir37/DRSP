//myApp.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>

char* myApp(MYSQL *conn, int patientID) {
    if (conn == NULL) {
        return strdup("{\"error\":\"Database connection failed\"}");
    }

    char query[1024];
    MYSQL_RES *result;
    MYSQL_ROW row;

    struct json_object *response_obj = json_object_new_object();

    // Fetch all appointments
    struct json_object *appointment_array = json_object_new_array();
    snprintf(query, sizeof(query),
        "SELECT a.AppointmentID, a.DoctorID, a.DateTime, a.Status, d.FullName, d.Spec, h.Name "
        "FROM appointments a "
        "JOIN doctors d ON a.DoctorID = d.DoctorID "
        "JOIN hospitals h ON d.HospitalID = h.HospitalID "
        "WHERE a.PatientID = %d AND a.DateTime > NOW() "
        "ORDER BY a.DateTime ASC",
        patientID);

    if (mysql_query(conn, query)) {
        return strdup("{\"error\":\"Database query failed for appointments\"}");
    }

    result = mysql_store_result(conn);
    if (result == NULL) {
        return strdup("{\"error\":\"Failed to retrieve appointments\"}");
    }

    while ((row = mysql_fetch_row(result))) {
        struct json_object *appointment_obj = json_object_new_object();
        json_object_object_add(appointment_obj, "AppointmentID", json_object_new_int(atoi(row[0])));
        json_object_object_add(appointment_obj, "DoctorID", json_object_new_int(atoi(row[1])));
        json_object_object_add(appointment_obj, "DateTime", json_object_new_string(row[2]));
        json_object_object_add(appointment_obj, "Status", json_object_new_string(row[3]));
        json_object_object_add(appointment_obj, "DoctorName", json_object_new_string(row[4]));
        json_object_object_add(appointment_obj, "DoctorSpec", json_object_new_string(row[5]));
        json_object_object_add(appointment_obj, "HospitalName", json_object_new_string(row[6]));
        json_object_array_add(appointment_array, appointment_obj);
    }
    json_object_object_add(response_obj, "Appointment", appointment_array);
    mysql_free_result(result);

    // Fetch all waitlist entries
    struct json_object *waitlist_array = json_object_new_array();
    snprintf(query, sizeof(query),
        "SELECT w.WaitlistID, w.DoctorID, w.Date, w.Status "
        "FROM waitlist w "
        "JOIN doctors d ON w.DoctorID = d.DoctorID "
        "JOIN hospitals h ON d.HospitalID = h.HospitalID "
        "WHERE w.PatientID = %d "
        "ORDER BY w.ModifiedDateTime DESC",
        patientID);

    if (mysql_query(conn, query)) {
        return strdup("{\"error\":\"Database query failed for waitlist\"}");
    }

    result = mysql_store_result(conn);
    if (result == NULL) {
        return strdup("{\"error\":\"Failed to retrieve waitlist\"}");
    }

    while ((row = mysql_fetch_row(result))) {
        struct json_object *waitlist_obj = json_object_new_object();
        json_object_object_add(waitlist_obj, "WaitlistID", json_object_new_int(atoi(row[0])));
        json_object_object_add(waitlist_obj, "DoctorID", json_object_new_int(atoi(row[1])));
        json_object_object_add(waitlist_obj, "Date", json_object_new_string(row[2]));
        json_object_object_add(waitlist_obj, "Status", json_object_new_string(row[3]));
        json_object_array_add(waitlist_array, waitlist_obj);
    }
    json_object_object_add(response_obj, "Waitlist", waitlist_array);
    mysql_free_result(result);

    // Convert the JSON object to a string and return
    char *response = strdup(json_object_to_json_string(response_obj));
    json_object_put(response_obj); // Free JSON object
    return response;
}
