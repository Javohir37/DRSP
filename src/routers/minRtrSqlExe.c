#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "../headers/sqlExe.h"

/**
 * Mini-router function for the sqlExe feature.
 *
 * @param json_request The incoming JSON request as a string.
 * @param response_buffer The buffer to store the response JSON.
 * @param buffer_size The size of the response buffer.
 */
void minRtrSqlExe(const char *json_request, char *response_buffer, size_t buffer_size) {
    struct json_object *parsed_json;
    struct json_object *args_obj;

    // Parse the incoming JSON request
    parsed_json = json_tokener_parse(json_request);
    if (parsed_json == NULL) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Invalid JSON format\"}");
        return;
    }

    // Extract "args" from JSON
    if (!json_object_object_get_ex(parsed_json, "args", &args_obj) || json_object_array_length(args_obj) < 1) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Missing or invalid arguments. Required: SQL query string.\"}");
        json_object_put(parsed_json);
        return;
    }

    // Extract the SQL query string
    const char *sql_query = json_object_get_string(json_object_array_get_idx(args_obj, 0));
    if (sql_query == NULL || strlen(sql_query) == 0) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"SQL query string is null or empty.\"}");
        json_object_put(parsed_json);
        return;
    }

    // Execute the SQL query
    sqlResult res = sqlExe(sql_query);

    // Prepare the response based on the result
    if (res.status == -1) {
        snprintf(response_buffer, buffer_size, "{\"error\": \"Failed to execute SQL query.\"}");
    } else if (res.result != NULL) {
        struct json_object *result_array = json_object_new_array();
        MYSQL_ROW row;
        MYSQL_FIELD *fields = mysql_fetch_fields(res.result);
        int num_fields = mysql_num_fields(res.result);

        int row_count = 0; // Counter for rows added to the JSON response

        // Build the JSON array from the SQL result
        while ((row = mysql_fetch_row(res.result)) && row_count < 5) { // Limit to 4 rows
            struct json_object *row_obj = json_object_new_object();
            for (int i = 0; i < num_fields; i++) {
                if (row[i]) {
                    json_object_object_add(row_obj, fields[i].name, json_object_new_string(row[i]));
                } else {
                    json_object_object_add(row_obj, fields[i].name, json_object_new_string(""));
                }
            }
            json_object_array_add(result_array, row_obj);
            row_count++;
        }

        // Convert JSON object to string and check buffer size
        const char *json_string = json_object_to_json_string(result_array);
        if (strlen(json_string) >= buffer_size) {
            snprintf(response_buffer, buffer_size, "{\"error\": \"Result exceeds buffer size even with row limit.\"}");
        } else {
            snprintf(response_buffer, buffer_size, "%s", json_string);
        }

        json_object_put(result_array); // Free the JSON object

        // Free the SQL result
        mysql_free_result(res.result);
    } else {
        snprintf(response_buffer, buffer_size, "{\"message\": \"No data returned.\"}");
    }

    json_object_put(parsed_json); // Free the parsed JSON object
}

