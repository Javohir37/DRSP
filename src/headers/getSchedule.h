#ifndef GET_SCHEDULE_H
#define GET_SCHEDULE_H

#include <mysql/mysql.h>

// Function to retrieve the schedule for a specific date and doctor
// Arguments:
// - conn: MySQL connection object
// - date: Date for which the schedule is needed (as a string)
// - doctorID: Doctor ID for whom the schedule is needed (as a string)
// Returns:
// - JSON string containing the schedule or an error message
char* getSchedule(MYSQL *conn, const char *date, const char *doctorID);

#endif // GET_SCHEDULE_H

