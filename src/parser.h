#ifndef PARSER_H
#define PARSER_H


#include <ctype.h>
#include<stdbool.h>
#include "table.h"

#define MAX_QUERY 256
#define MAX_TOKENS 64

void parse_sql(char *query);
char *trim(char *str);
char *unquote(char *str);

#endif