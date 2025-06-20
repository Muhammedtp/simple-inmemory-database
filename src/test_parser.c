#include"parser.h"

bool parse_query(char *query){
    if (!query || strlen(query) == 0) {
        printf("Error: Empty query.\n");
        return;
    }

    char query_copy[MAX_QUERY];
    strncpy(query_copy, query, MAX_QUERY - 1);
    query_copy[MAX_QUERY - 1] = '\0';

    char *tokens[MAX_TOKENS];
    int token_count = 0;

    char *ptr = query_copy;
    char current_token[256];
    int token_pos = 0;
    bool in_quotes = false;

    while(*ptr && token_count<MAX_TOKENS){
        if(*ptr=='\'' && !in_quotes ){
            in_quotes = true;
            current_token[token_pos++] = *ptr;
        } else if(*ptr == '\'' && in_quotes){
            in_quotes = false;
            current_token[token_pos] = *ptr;
            curr
        }
        else{
            current_token[token_pos++] = *ptr;
        }
        ptr++;
    }
}