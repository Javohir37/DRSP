#ifndef SERVER_H
#define SERVER_H

// Starts the server, listens for incoming client connections,
// and spawns a dedicated thread for each connected client.
void start_server();

#endif // SERVER_H
