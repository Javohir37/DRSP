#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

// Append a JSON-safe string to the destination buffer.
void append_json_string(char *dest, const char *str, size_t *pos) {
    dest[*pos] = '"';
    (*pos)++;
    for (const char *p = str; *p != '\0'; p++) {
        dest[*pos] = *p;
        (*pos)++;
    }
    dest[*pos] = '"';
    (*pos)++;
}

// Generate JSON string from the result set.
char* generate_json(char ***all_rows, MYSQL_FIELD *fields, int row_count, int num_fields) {
    size_t buffer_size = 1024 * 1024; // 1 MB buffer size
    char *json = (char *)malloc(buffer_size);
    size_t pos = 0;

    if (json == NULL) {
        fprintf(stderr, "Memory allocation failed for JSON buffer\n");
        exit(1);
    }

    json[pos++] = '[';

    for (int rr = 0; rr < row_count; rr++) {
        if (rr > 0) {
            json[pos++] = ',';
        }
        json[pos++] = '{';

        for (int i = 0; i < num_fields; i++) {
            append_json_string(json, fields[i].name, &pos);
            json[pos++] = ':';

            if (all_rows[rr][i] == NULL) {
                const char *null_str = "null";
                strcpy(&json[pos], null_str);
                pos += strlen(null_str);
            } else {
                append_json_string(json, all_rows[rr][i], &pos);
            }

            if (i < num_fields - 1) {
                json[pos++] = ',';
            }
        }

        json[pos++] = '}';
    }

    json[pos++] = ']';
    json[pos] = '\0';

    return json;
}

