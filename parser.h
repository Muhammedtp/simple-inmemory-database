#ifndef PARSER_H
#define PARSER_H

#include "table.h"
#include <ctype.h>
#include<stdbool.h>

#define MAX_QUERY 256
#define MAX_TOKENS 32

void parse_sql(char *query);
char *trim(char *str);
char *unquote(char *str);

#endif