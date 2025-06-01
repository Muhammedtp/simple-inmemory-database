#include "parser.h"

void parse_sql(char *query) {
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
    
    while (*ptr && token_count < MAX_TOKENS) {
        if (*ptr == '\'' && !in_quotes) {
            in_quotes = true;
            current_token[token_pos++] = *ptr;
        } else if (*ptr == '\'' && in_quotes) {
            in_quotes = false;
            current_token[token_pos++] = *ptr;
            current_token[token_pos] = '\0';
            
            tokens[token_count] = malloc(strlen(current_token) + 1);
            strcpy(tokens[token_count], trim(current_token));
            token_count++;
            
            token_pos = 0;
        } else if (in_quotes) {
            current_token[token_pos++] = *ptr;
        } else if (strchr(" ,()=;", *ptr)) {
            if (token_pos > 0) {
                current_token[token_pos] = '\0';
                tokens[token_count] = malloc(strlen(current_token) + 1);
                strcpy(tokens[token_count], trim(current_token));
                token_count++;
                token_pos = 0;
            }
        } else {
            current_token[token_pos++] = *ptr;
        }
        ptr++;
    }
    
    if (token_pos > 0) {
        current_token[token_pos] = '\0';
        tokens[token_count] = malloc(strlen(current_token) + 1);
        strcpy(tokens[token_count], trim(current_token));
        token_count++;
    }
    
    if (token_count == 0) {
        printf("Error: Invalid query.\n");
        return;
    }
    
    if (strcmp(tokens[0], "CREATE") == 0 && token_count >= 5 && strcmp(tokens[1], "TABLE") == 0) {
        char *table_name = tokens[2];
        int col_count = 0;
        const char *columns[MAX_COLUMNS];
        for (int i = 3; i < token_count; i++) {
            if (col_count >= MAX_COLUMNS) {
                printf("Error: Too many columns.\n");
                return;
            }
            columns[col_count++] = tokens[i];
        }
        create_table(table_name, col_count, columns);
    }
    
 
    else if (strcmp(tokens[0], "INSERT") == 0 && token_count >= 5 && strcmp(tokens[1], "INTO") == 0 && strcmp(tokens[3], "VALUES") == 0) {
        char *table_name = tokens[2];
        int val_count = 0;
        char *values[MAX_COLUMNS];
        for (int i = 4; i < token_count; i++) {
            if (val_count >= MAX_COLUMNS) {
                printf("Error: Too many values.\n");
                return;
            }
            values[val_count++] = unquote(tokens[i]);
        }
        insert(table_name, values, val_count);
    }
    
    
    else if (strcmp(tokens[0], "SELECT") == 0 && token_count >= 4 && strcmp(tokens[1], "*") == 0 && strcmp(tokens[2], "FROM") == 0) {
    char *table_name = tokens[3];
    
    
    
    bool has_where = false;
    int where_index = -1;
    
    for (int i = 4; i < token_count; i++) {
        if (strcmp(tokens[i], "WHERE") == 0) {
            has_where = true;
            where_index = i;
            break;
        }
    }
    
    if (has_where && where_index != -1) {
        
        if (where_index + 2 < token_count) {  
            char *column_name = tokens[where_index + 1];
            char *value = NULL;
            
            for (int i = where_index + 2; i < token_count; i++) {
                if (strcmp(tokens[i], "=") != 0 && strlen(tokens[i]) > 0) {
                    value = unquote(tokens[i]);
                    break;
                }
            }
            
            if (value && strlen(value) > 0) {
                printf("DEBUG: WHERE %s = %s\n", column_name, value);
                query_rows(table_name, column_name, value);
            } else {
                printf("Error: No value found in WHERE clause.\n");
            }
        } else {
            printf("Error: Incomplete WHERE clause.\n");
        }
    } else {
        
        query_all_rows(table_name);
    }
}
    else if (strcmp(tokens[0], "UPDATE") == 0 && token_count >= 8 && strcmp(tokens[2], "SET") == 0) {
        char *table_name = tokens[1];
        char *target_column = tokens[3];
        
    
        char *new_value = NULL;
        int where_index = -1;
        
        for (int i = 4; i < token_count; i++) {
            if (strcmp(tokens[i], "WHERE") == 0) {
                where_index = i;
                break;
            } else if (strcmp(tokens[i], "=") != 0) {
                new_value = unquote(tokens[i]);
            }
        }
        
        if (where_index > 0 && new_value && where_index + 2 < token_count) {
            char *cond_column = tokens[where_index + 1];
            char *cond_value = NULL;
            
            for (int i = where_index + 2; i < token_count; i++) {
                if (strcmp(tokens[i], "=") != 0) {
                    cond_value = unquote(tokens[i]);
                    break;
                }
            }
            
            if (cond_value) {
                update_rows(table_name, cond_column, cond_value, target_column, new_value);
            } else {
                printf("Error: Invalid UPDATE WHERE clause.\n");
            }
        } else {
            printf("Error: Invalid UPDATE syntax.\n");
        }
    }
    
    else if (strcmp(tokens[0], "DELETE") == 0 && token_count >= 6 && strcmp(tokens[1], "FROM") == 0 && strcmp(tokens[3], "WHERE") == 0) {
        char *table_name = tokens[2];
        char *column_name = tokens[4];
        char *value = NULL;
        
        for (int i = 5; i < token_count; i++) {
            if (strcmp(tokens[i], "=") != 0) {
                value = unquote(tokens[i]);
                break;
            }
        }
        
        if (value) {
            delete_rows(table_name, column_name, value);
        } else {
            printf("Error: Invalid DELETE WHERE clause.\n");
        }
    }

    else if (strcmp(tokens[0], "DROP") == 0 && token_count >= 3 && strcmp(tokens[1], "TABLE") == 0) {
        char *table_name = tokens[2];
        drop_table(table_name);
        return;
    }
    else{
        printf("Error: Unsupported or invalid SQL command.\n");
    }

    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
}

char *trim(char *str) {
    while (isspace(*str)) str++;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) *end-- = '\0';
    return str;
}

char *unquote(char *str) {
    if (str && str[0] == '\'' && str[strlen(str) - 1] == '\'') {
        str[strlen(str) - 1] = '\0';
        return str + 1;
    }
    return str;
}