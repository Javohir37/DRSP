#include <stdio.h>
#include <string.h>
#include "../headers/bigBossRouter.h"
#include "../headers/localmysql.h"

int main() {
    MYSQL *conn = establish_connection();
    if (!conn) {
        fprintf(stderr, "Database connection failed.\n");
        return 1;
    }

    char json_request[4096];  // Buffer for the input JSON request
    char response_buffer[8192];  // Buffer for the output response
    size_t buffer_size = sizeof(response_buffer);

    printf("Enter a single-line JSON request (or 'exit' to quit):\n");

    while (fgets(json_request, sizeof(json_request), stdin)) {
        json_request[strcspn(json_request, "\n")] = '\0';  // Remove trailing newline

        if (strcmp(json_request, "exit") == 0) {
            break;
        }

        // Debug: Print the raw input
        printf("DEBUG: Raw input JSON: '%s'\n", json_request);

        bigBossRouter(json_request, response_buffer, buffer_size);

        // Print the response
        printf("%s\n", response_buffer);
    }

    mysql_close(conn);
    return 0;
}

