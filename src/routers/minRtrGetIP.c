#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "localmysql.h"
#include "getIP.h"

/**
 * minRtrGetIP
 * 
 * This function acts as a mini-router for handling requests
 * to retrieve IP addresses and port numbers from the database.
 * It expects a JSON object containing "args" and calls getIP to fetch
 * available addresses, then returns the results in JSON format.
 *
 * @param json_request    A string containing the JSON request
 * @param response_buffer A buffer to store the JSON response
 * @param buffer_size     Size of the response buffer
 */
void minRtrGetIP(const char *json_request, char *response_buffer, size_t buffer_size) {
    // Parse the incoming JSON request
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    // Attempt to extract the "args" field from the parsed JSON
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj)) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing 'args' field\"}");
        json_object_put(parsed_json);
        return;
    }

    // Ensure we have the required arguments in the "args" array
    if (json_object_array_length(args_obj) < 1) {
        snprintf(response_buffer, buffer_size,
                 "{\"error\": \"Missing or invalid arguments. Required: status\"}");
        json_object_put(parsed_json);
        return;
    }

    // Extract status argument from the JSON array
    const char *status = json_object_get_string(json_object_array_get_idx(args_obj, 0));

    // Establish a connection to the database
    MYSQL *conn = establish_connection();
    if (!conn) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Database connection failed\"}");
        json_object_put(parsed_json);
        return;
    }

    // Call getIP to retrieve IP addresses from the 'ip' table
    char *result = getIP(conn, status);
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to process getIP\"}");
    }

    // Clean up: close the DB connection and free the parsed JSON object
    mysql_close(conn);
    json_object_put(parsed_json);
}
