// joinWaitlist.h
#ifndef JOINWAITLIST_H
#define JOINWAITLIST_H

#include <mysql/mysql.h>

// Function prototype for joinWaitlist
int joinWaitlist(MYSQL *con, int PatientID, int DoctorID, const char *Date);

#endif

