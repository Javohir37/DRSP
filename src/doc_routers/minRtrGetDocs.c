#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include "getDocs.h"
#include "localmysql.h"

void minRtrGetDocs(MYSQL *conn, const char *json_request, char *response_buffer, size_t buffer_size) {
    printf("DEBUG: Inside minRtrGetDocs\n");

    // Parse the input JSON
    struct json_object *parsed_json = json_tokener_parse(json_request);
    if (!parsed_json) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    // Extract arguments
    struct json_object *args_obj;
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 2) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments for 'getDocs'\"}");
        json_object_put(parsed_json);
        return;
    }

    // Get arguments: HospitalID and Spec
    int hospitalID = json_object_get_int(json_object_array_get_idx(args_obj, 0));
    const char *spec = json_object_get_string(json_object_array_get_idx(args_obj, 1));

    printf("DEBUG: HospitalID: %d, Spec: %s\n", hospitalID, spec);

    // Call getDocs and handle the result
    char *result = getDocs(conn, hospitalID, spec);
    if (result) {
        snprintf(response_buffer, buffer_size, "%s", result);
        free(result); // Free the dynamically allocated JSON string
    } else {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to retrieve doctors\"}");
    }

    json_object_put(parsed_json);
}

