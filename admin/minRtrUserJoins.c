#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "localmysql.h"
#include "userJoins.h"

/**
 * minRtrUserJoins
 * 
 * This function acts as a mini-router for handling requests
 * to insert a new user join entry into the IP table.
 * It expects a JSON object containing "args" with IP and port.
 *
 * @param json_request    A string containing the JSON request
 * @param response_buffer A buffer to store the JSON response
 * @param buffer_size     Size of the response buffer
 */
void minRtrUserJoins(const char *json_request, char *response_buffer, size_t buffer_size) {
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
    if (json_object_array_length(args_obj) < 2) {
        snprintf(response_buffer, buffer_size,
                 "{\"error\": \"Missing or invalid arguments. Required: IP, Port\"}");
        json_object_put(parsed_json);
        return;
    }

    // Extract IP and Port arguments from the JSON array
    const char *ip = json_object_get_string(json_object_array_get_idx(args_obj, 0));
    const char *port = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    // Call userJoins to insert the new entry into the 'ip' table
    userJoins(ip, port);

    // Set a success response
    snprintf(response_buffer, buffer_size, "{\"status\": \"User join entry inserted successfully\"}");

    // Clean up the parsed JSON object
    json_object_put(parsed_json);
}
