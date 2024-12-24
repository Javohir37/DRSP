#ifndef USERJOINS_H
#define USERJOINS_H

/**
 * Function to insert a new user join entry into the IP table with status 'sent'.
 *
 * @param ip The IP address of the user.
 * @param port The port number of the user.
 */
void userJoins(const char *ip, const char *port);

#endif // USERJOINS_H
