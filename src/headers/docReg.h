#ifndef DOC_REG_H
#define DOC_REG_H

#include <mysql/mysql.h>

/**
 * Registers a doctor in the database.
 * 
 * @param conn        A valid, open MySQL connection object.
 * @param fullName    The full name of the doctor.
 * @param spec        The doctor's specialty.
 * @param role        The doctor's role.
 * @param hospitalID  The ID of the hospital associated with the doctor.
 * @param phoneNumber The doctor's phone number.
 * @param email       The doctor's email address.
 * @param password    The doctor's login password.
 * @return            A dynamically allocated JSON string with the new DoctorID,
 *                    or NULL on error.
 */
char* docReg(MYSQL *conn,
             const char *fullName,
             const char *spec,
             const char *role,
             int hospitalID,
             const char *phoneNumber,
             const char *email,
             const char *password);

#endif // DOC_REG_H
