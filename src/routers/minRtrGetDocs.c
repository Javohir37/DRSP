#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "../headers/minRtrGetDocs.h"
#include "../headers/localmysql.h" // For establish_connection
#include "../headers/getDocs.h"    // Include the header where getDocs is declared

void minRtrGetDocs(const char *json_request, char *response_buffer, size_t buffer_size) {
    struct json_object *parsed_json;
    struct json_object *args_obj;

    // Parse the incoming JSON request
    parsed_json = json_tokener_parse(json_request);
    if (parsed_json == NULL) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    // Extract "args" from JSON
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 2) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments. Required: hospitalID and specialization.\"}");
        json_object_put(parsed_json);
        return;
    }

    // Extract hospitalID and specialization from the JSON array
    const char *hospitalID = json_object_get_string(json_object_array_get_idx(args_obj, 0));
    const char *specialization = json_object_get_string(json_object_array_get_idx(args_obj, 1));
    if (!hospitalID || !specialization) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid hospitalID or specialization.\"}");
        json_object_put(parsed_json);
        return;
    }

    // Establish a connection to the database
    MYSQL *conn = establish_connection(); // Ensure this function is defined in localmysql.h
    if (conn == NULL) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to connect to the database.\"}");
        json_object_put(parsed_json);
        return;
    }

    // Call the getDocs function
    char *result = getDocs(conn, hospitalID, specialization);
    if (result == NULL) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve doctor details.\"}");
    } else {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result); // Free the allocated JSON string
    }

    // Close the database connection
    mysql_close(conn);

    json_object_put(parsed_json); // Free the parsed JSON object
}

