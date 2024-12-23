// jsonParser.h
#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <mysql/mysql.h>
#include <stddef.h>

void append_json_string(char *dest, const char *str, size_t *pos);
char* generate_json(char ***all_rows, MYSQL_FIELD *fields, int row_count, int num_fields);

#endif // JSONPARSER_H

