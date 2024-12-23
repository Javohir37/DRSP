// minRtrModifAppoint.h

#ifndef MIN_RTR_MODIF_APPOINT_H
#define MIN_RTR_MODIF_APPOINT_H

#include <mysql/mysql.h>

// Function to handle JSON request for modifying appointment status
char* handleModifAppointRequest(MYSQL *conn, const char *requestJSON);

#endif // MIN_RTR_MODIF_APPOINT_H

