#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <json-c/json.h>

#include "../headers/bigBossRouter.h"
#include "server.h"

// Maximum number of concurrent client connections.
#define MAX_CLIENTS 10
// Size of the buffer for incoming and outgoing data.
#define BUFFER_SIZE 2048

static int client_count = 0;
static int client_sockets[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function for handling an individual client.
void *handle_client(void *arg) {
    int client_sd = *((int *)arg);
    free(arg);

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    while (1) {
        // Receive data from the client.
        int bytes_received = recv(client_sd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            // Client disconnected or error on recv.
            pthread_mutex_lock(&clients_mutex);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == client_sd) {
                    client_sockets[i] = 0;
                    break;
                }
            }
            client_count--;
            pthread_mutex_unlock(&clients_mutex);

            printf("Client #%d disconnected.\n", client_sd);
            close(client_sd);
            return NULL;
        }

        // Null-terminate the received data for safe string handling.
        buffer[bytes_received] = '\0';
        printf("Received from Client #%d: %s\n", client_sd, buffer);

        // Check if the data is valid JSON and route it.
        struct json_object *parsed_json = json_tokener_parse(buffer);
        if (parsed_json) {
            // Pass the received string to bigBossRouter.
            bigBossRouter(buffer, response, sizeof(response));
            // Send the result from bigBossRouter back to the client.
            send(client_sd, response, strlen(response), 0);
            json_object_put(parsed_json);
        } else {
            // If the data is invalid JSON, send an error message.
            const char *error_message = "{\"error\": \"Invalid JSON format\"}";
            send(client_sd, error_message, strlen(error_message), 0);
        }
    }

    return NULL;
}

void start_server() {
    int server_sd;
    struct sockaddr_in server_addr;

    // Create the socket.
    server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Reuse the address if the program restarts quickly.
    int opt = 1;
    if (setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_sd);
        exit(EXIT_FAILURE);
    }

    // Prepare the server socket address structure.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the address to the socket.
    if (bind(server_sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_sd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections.
    if (listen(server_sd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_sd);
        exit(EXIT_FAILURE);
    }

    printf("Server started on port 9999...\n");

    // Initialize the client sockets array.
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        // Accept an incoming client connection.
        int client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sd < 0) {
            perror("Accept failed");
            continue;
        }

        // Print a message indicating a new connection.
        printf("New client connected: %d\n", client_sd);
        // Check if there is room for this client.
        pthread_mutex_lock(&clients_mutex);
        if (client_count >= MAX_CLIENTS) {
            pthread_mutex_unlock(&clients_mutex);
            printf("Max clients reached. Connection rejected.\n");
            close(client_sd);
            continue;
        }

        // Find a spot in the client_sockets array for the new client.
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_sd;
                break;
            }
        }
        client_count++;
        pthread_mutex_unlock(&clients_mutex);

        // Create a new thread to handle this client.
        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        if (!pclient) {
            perror("Unable to allocate memory for client");
            close(client_sd);
            continue;
        }
        *pclient = client_sd;
        pthread_create(&tid, NULL, handle_client, pclient);
        // Detach the thread so that resources are reclaimed when it finishes.
        pthread_detach(tid);
    }

    close(server_sd);
}
