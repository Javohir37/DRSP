// joinWaitlist.c
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <time.h>
#include "../headers/joinWaitlist.h"

// Helper function to get the current datetime
void get_current_datetime(char *datetime) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Function to add a patient to the waitlist
int joinWaitlist(MYSQL *con, int PatientID, int DoctorID, const char *Date) {
    char query[512];
    char current_datetime[20];

    // Get the current datetime for ModifiedDateTime
    get_current_datetime(current_datetime);

    // Construct the query to insert the new waitlist record
    snprintf(query, sizeof(query), "INSERT INTO waitlist (PatientID, DoctorID, Date, Status, ModifiedDateTime) "
                                   "VALUES (%d, %d, '%s', 'Confirmed', '%s')", 
                                   PatientID, DoctorID, Date, current_datetime);

    // Execute the query
    if (mysql_query(con, query)) {
        fprintf(stderr, "INSERT failed: %s\n", mysql_error(con));
        return -1; // Error occurred
    }

    return 0; // Success
}

