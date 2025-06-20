#include "table.h"

struct DataBase DB = {0};


int free_table_index(){
    for(int i=0;i<MAX_TABLES;i++){
        if(DB.tables[i]==NULL){
            return i;
        }
    }
    return -1;
}

void create_table(char *name, const int num_columns, char *columns[], int datatypes[]) {
    if (!name || strlen(name) >= STRING_SIZE - 1 || num_columns > MAX_COLUMNS || num_columns <= 0 || DB.num_tables >= MAX_TABLES) {
        printf("Error: Invalid input parameters.\n");
        return;
    }
    
    for(int i = 0; i < num_columns; i++) {
        if (!columns[i] || strlen(columns[i]) >= STRING_SIZE - 1) {
            printf("Error: Invalid column name at index %d.\n", i);
            return;
        }
        if (datatypes[i] < String || datatypes[i] > Date) {
            printf("Error: Unsupported datatype at column %d.\n", i);
            return;
        }
        for (int j = i + 1; j < num_columns; j++) {
            if (strcmp(columns[i], columns[j]) == 0) {
                printf("Error: Duplicate column name '%s' at indices %d and %d.\n", 
                       columns[i], i, j);
                return;
            }
        }
    }
    
    if (find_table(name) != -1) {
        printf("Error: Table '%s' already exists.\n", name);
        return;
    }

    int index = free_table_index();
    if(index == -1) {
        printf("Error: Maximum number of tables reached.\n");
        return;
    }
    
    TableMetaData *table = (TableMetaData*) malloc(sizeof(TableMetaData));
    if(table == NULL) {
        printf("Error: Memory allocation failed for table metadata.\n");
        return;
    }

    DB.tables[index] = table;
    
    strcpy(table->name, name);
    table->num_rows = 0;
    table->num_cols = num_columns;
    
    size_t row_size = 0;
    table->offset[0] = 0;
    
    for(int i = 0; i < num_columns; i++) {
        table->datatypes[i] = datatypes[i];
        strcpy(table->columns[i], columns[i]);
        
        size_t column_size;

        switch (datatypes[i]){
            case String: {
                column_size = sizeof(char) * STRING_SIZE;
                break;
            }
            case Integer: {
                column_size = sizeof(int);
                break;
            }
            case Character: {
                column_size = sizeof(char);
                break;
            }
            case Bool: {
                column_size = sizeof(bool);
                break;
            } 
            case Float: {
                column_size = sizeof(float);
                break;
            }
            case Date: {
                column_size = sizeof(datetype);
                break;
            } 
            default: {
                printf("Error: Unknown data type %d for column %d.\n", datatypes[i], i);
                free(table);
                DB.tables[index] = NULL;
                return;
            }

        }
        
        row_size += column_size;
        
        if(i < num_columns - 1) {
            table->offset[i + 1] = table->offset[i] + column_size;
        }
    }
    
    table->rowsize = row_size;
    table->memory = NULL;  
    DB.num_tables++;

    printf("Table '%s' created successfully with %d columns.\n", name, num_columns);
}


int find_table(char *name){
    if (!name) {
        return -1;
    }
    for (int i = 0; i < MAX_TABLES; i++) {
        if(DB.tables[i] != NULL && strcmp(DB.tables[i]->name, name) == 0){
            return i; 
        }
    }
    return -1; 
}

int if_match(TableMetaData *table,int col_in,char *value,int row_in){
    if(!table || !value || col_in < 0 || col_in >= table->num_cols || 
        row_in < 0 || row_in >= table->num_rows){
        return 0;
    }
    if (!table->memory || !table->memory[row_in]) {
        return 0; 
    }
    char *row_data = (char*)table->memory[row_in];
    void *cell_data = row_data + table->offset[col_in];

    
    int match = 0;
    
    switch(table->datatypes[col_in]){
        case Integer: {
            int cell_value = *(int*)cell_data;
            int search_value = atoi(value);
            match = (cell_value == search_value);
            break;
        }
        case Character: {
            char cell_value = *(char*)cell_data;
            char search_value = value[0];
            match = (cell_value == search_value);
            break;
        }
        case String: {
            char *cell_value = (char*)cell_data;
            match = (strcmp(cell_value, value) == 0);
            break;
        }
        case Bool: {
            bool cell_value = *(bool*)cell_data; // Fixed: was casting to char*
            bool search_val = false;
            
            if (strcmp(value, "true") == 0 || strcmp(value, "TRUE") == 0 ||
                strcmp(value, "1") == 0 || strcmp(value, "yes") == 0 ||
                strcmp(value, "YES") == 0) {
                search_val = true;
            } else if (strcmp(value, "false") == 0 || strcmp(value, "FALSE") == 0 ||
                      strcmp(value, "0") == 0 || strcmp(value, "no") == 0 ||
                      strcmp(value, "NO") == 0) {
                search_val = false;
            } else {
                return 0;
            }
            
            match = (cell_value == search_val);
            break;
        }
        case Float: {
            float cell_val = *(float*)cell_data;
            float search_val = atof(value);
            match = (fabs(cell_val - search_val) < 1e-6);
            break;
        }
        case Date: {
             datetype cell_val = *(datetype*)cell_data;
            datetype search_val = parse_date_string(value);
            match = (cell_val==search_val);
            break;
        }
    }
    return match;
}

bool insert(char *table_name, char *values[], const int num_cols) {
    if (!table_name || !values) {
        printf("Error: Null table name or values.\n");
        return false;
    }
    
    int ind = find_table(table_name);
    if (ind == -1) {
        printf("Error: Table '%s' not found.\n", table_name);
        return false;
    }
    
    if (num_cols != DB.tables[ind]->num_cols) {
        printf("Error: Number of values (%d) does not match number of columns (%d).\n",  num_cols, DB.tables[ind]->num_cols);
        return false;
    }
    
    if (DB.tables[ind]->num_rows >= MAX_ROWS) {
        printf("Error: Maximum number of rows exceeded.\n");
        return false;
    }
    
    for (int i = 0; i < num_cols; i++) {
        if (!values[i]) {
            printf("Error: Null value at column %d.\n", i);
            return false;
        }
    }
    
    TableMetaData *table = DB.tables[ind];
    int current_row = table->num_rows;

    
    if (table->memory == NULL) {
        table->memory = (void**)malloc(sizeof(void*) * MAX_ROWS);
        if (table->memory == NULL) {
            printf("Error: Failed to allocate memory for row pointers.\n");
            return false;
        }
    }
    
    table->memory[current_row] = malloc(table->rowsize);
    if (table->memory[current_row] == NULL) {
        printf("Error: Failed to allocate memory for row data.\n");
        return false;
    }
    
    char *row_data = (char*)table->memory[current_row];
    
    for (int i = 0; i < num_cols; i++) {
        void *cell_location = row_data + table->offset[i];

        switch(table->datatypes[i]){
                    case Integer: {
                        char *endptr;
                        long value = strtol(values[i], &endptr, 10);
                        if (*endptr != '\0' || value > INT_MAX || value < INT_MIN) {
                            printf("Error: Invalid integer value '%s'.\n", values[i]);
                            free(table->memory[current_row]);
                            return false;
                        }
                        *(int*)cell_location = (int)value;
                        break;
                    }
                    case Character: {
                        *(char*)cell_location = values[i][0];
                        break;
                    }
                    case String: {
                        strncpy((char*)cell_location, values[i], STRING_SIZE - 1);
                        ((char*)cell_location)[STRING_SIZE - 1] = '\0'; 
                        break;
                    }
                    case Bool: {
                        if(strcmp(values[i], "true") == 0 || strcmp(values[i], "TRUE") == 0 || 
                            strcmp(values[i], "1") == 0 || strcmp(values[i], "yes") == 0 || 
                            strcmp(values[i], "YES") == 0){
                            *(bool*)cell_location = true;
                        }else if (strcmp(values[i], "false") == 0 || strcmp(values[i], "FALSE") == 0 || 
                                strcmp(values[i], "0") == 0 || strcmp(values[i], "no") == 0 || 
                                strcmp(values[i], "NO") == 0) {
                            *(bool*)cell_location = false;
                        } else {
                            printf("Error: Invalid boolean value");
                            free(table->memory[current_row]);
                            table->memory[current_row] = NULL;
                            return false;
                        }   
                        break;
                    }
                    case Float: {
                        char *endptr;
                        float float_value = strtof(values[i], &endptr);
                        if (*endptr != '\0') {
                            printf("Error: Invalid float value '%s'.\n", values[i]);
                            free(table->memory[current_row]);
                            return false;
                        }
                        *(float*)cell_location = float_value;
                        break;
                    }
                    case Date: {
                        datetype date_value = parse_date_string(values[i]);
                        if(date_value==0){
                            printf("Error: Invalid date value '%s' for column %d.\n", values[i], i);
                            free(table->memory[current_row]); // Clean up allocated memory
                            table->memory[current_row] = NULL;
                            return false;
                        }
                        *(datetype*)cell_location = date_value;
                        break;
                    }
                }
            }
    
    table->num_rows++;
    printf("Row inserted successfully into table '%s'. Total rows: %d\n", table_name, table->num_rows);
    return true;
}


void query_rows(char *table_name, char *column_name, char *value) {
    if (!table_name || !column_name || !value) {
        printf("Error: Null table name, column name, or value.\n");
        return;
    }
    int ind = find_table(table_name);
    if (ind == -1) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }
    TableMetaData *table = DB.tables[ind];
    if (table->num_rows == 0) {
        printf("Error: Table '%s' is empty.\n", table_name);
        return;
    }
    int col_index = -1;
    int dt;
    for (int i = 0; i < table->num_cols; i++) {
        if (strcmp(table->columns[i], column_name) == 0) {
            col_index = i;
            dt = table->datatypes[i];
            break;
        }
    }
    if (col_index == -1) {
        printf("Error: Column '%s' not found.\n", column_name);
        return;
    }
    int found = 0;
    printf("Table: %s\n",table_name);
    for(int i=0;i<table->num_cols;i++){
        printf("%-15s\t",table->columns[i]);
    }
    printf("\n");
    for (int i = 0; i < table->num_cols; i++) {
        printf("---------------");
    }
    printf("\n");

    for (int i = 0; i < table->num_rows; i++) {
        
        if (if_match(table,col_index,value,i)) {
            found++;
            char *row_data = (char*)table->memory[i];

            for (int j = 0; j < table->num_cols; j++) {
                void *cell_location = row_data + table->offset[j];
                
                switch(table->datatypes[j]){
                    case Integer: {
                        int val = *(int*)cell_location;
                        printf("%-15d\t", val);
                        break;
                    }
                    case Character: {
                        char val = *(char*)cell_location;
                        printf("%-15c\t", val);
                        break;
                    }
                    case String: {
                        char *val = (char*)cell_location;
                        printf("%-15s\t", val);
                        break;
                    }
                    case Bool: {
                        bool val = *(bool*)cell_location;
                        printf("%-15s\t", val ? "true" : "false");
                        break;
                    }
                    case Float: {
                        float val = *(float*)cell_location;
                        printf("%-15f\t", val);
                        break;
                    }
                    case Date: {
                       datetype val = *(datetype*)cell_location;
                        print_date(val);
                        break;
                    }
                }
            }
            printf("\n");
        }
    }
    
    if (!found) {
        printf("No rows found with %s = %s.\n", column_name, value);
    } else {
        printf("\nFound %d matching row(s).\n", found);
    }
}

void drop_table(char *table){
    int ind = find_table(table);
    if(ind==-1){
        printf("Error: Table does not exists\n");
        return;
    }
    for(int i=0;i<DB.tables[ind]->num_rows;i++){
        free(DB.tables[ind]->memory[i]);
    }
    free(DB.tables[ind]->memory);
    free(DB.tables[ind]);
    DB.tables[ind] =NULL;
    DB.num_tables--;
    return;
}

void free_database() {
    for (int i = 0; i < MAX_TABLES; i++) {
        if(DB.tables[i]!=NULL){
            drop_table(DB.tables[i]->name);
        }
    }
    DB.num_tables = 0;
}




void delete_rows(char *table_name, char *column_name, char *value) {
    if (!table_name || !column_name || !value) {
        printf("Error: Null table name, column name, or value.\n");
        return;
    }
    int ind = find_table(table_name);
    if (ind == -1) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }
    int col_index = -1;
    TableMetaData *table = DB.tables[ind];
    for (int i = 0; i < table->num_cols; i++) {
        if (strcmp(table->columns[i], column_name) == 0) {
            col_index = i;
            break;
        }
    }
    if (col_index == -1) {
        printf("Error: Column '%s' not found.\n", column_name);
        return;
    }
    int deleted = 0;

    for (int i = 0; i < table->num_rows; i++) {
        
        if (if_match(table,col_index,value,i)) {
            free(table->memory[i]);
            for(int j=i;j<table->num_rows-1;j++){
                table->memory[j] = table->memory[j+1];
            }
            table->num_rows--;
            deleted++;
            i--;
        }

    }
    
    if (deleted > 0) {
        printf("%d row(s) deleted from %s where %s = %s.\n", deleted, table_name, column_name, value);
    } else {
        printf("No rows found with %s = %s.\n", column_name, value);
    }
}

bool update_rows(char *table_name, char *cond_column, char *cond_value, char *target_column, char *new_value) {
    if (!table_name || !cond_column || !cond_value || !target_column || !new_value) {
        printf("Error: Null table name, column name, or value.\n");
        return false;
    }
    if (strlen(new_value) >= STRING_SIZE - 1) {
        printf("Error: New value too long.\n");
        return false;
    }
    int ind = find_table(table_name);
    if (ind == -1) {
        printf("Error: Table '%s' not found.\n", table_name);
        return false;
    }
    TableMetaData *table = DB.tables[ind];
    int cond_col_index = -1, target_col_index = -1;
    for (int i = 0; i < table->num_cols; i++) {
        if (strcmp(table->columns[i], cond_column) == 0) {
            cond_col_index = i;
        }
        if (strcmp(table->columns[i], target_column) == 0) {
            target_col_index = i;
        }
    }
    if (cond_col_index == -1) {
        printf("Error: Condition column '%s' not found.\n", cond_column);
        return false;
    }
    if (target_col_index == -1) {
        printf("Error: Target column '%s' not found.\n", target_column);
        return false;
    }
    int updated = 0;
    for (int r = 0; r < table->num_rows; r++) {
        if (if_match(table, cond_col_index, cond_value, r)) {
            void *cell = (char*)table->memory[r] + table->offset[target_col_index];
            switch(table->datatypes[target_col_index]){
                    case Integer: {
                        char *endptr;
                        long value = strtol(new_value, &endptr, 10);
                        if (*endptr != '\0' || value > INT_MAX || value < INT_MIN) {
                        printf("Error: Invalid integer value '%s'.\n", new_value);
                        return false;
                        }
                        *(int*)cell = (int)value;
                        break;
                    }
                    case Character: {
                        char value = new_value[0];
                        *(char *)cell = value;
                        break;
                    }
                    case String: {
                        char *ptr = (char*)cell;
                        strncpy(ptr, new_value, STRING_SIZE - 1);
                        ptr[STRING_SIZE - 1] = '\0';
                        break;
                    }
                    case Bool: {
                        if(strcmp(new_value, "true") == 0 || strcmp(new_value, "TRUE") == 0 || 
                            strcmp(new_value, "1") == 0 || strcmp(new_value, "yes") == 0 || 
                            strcmp(new_value, "YES") == 0){
                            *(bool*)cell = true;
                        }else if (strcmp(new_value, "false") == 0 || strcmp(new_value, "FALSE") == 0 || 
                                strcmp(new_value, "0") == 0 || strcmp(new_value, "no") == 0 || 
                                strcmp(new_value, "NO") == 0) {
                            *(bool*)cell = false;
                        } else {
                            printf("Error: Invalid boolean value");
                            return false;
                        }   
                        break;
                    }
                    case Float: {
                        char *endptr;
                        float float_value = strtof(new_value, &endptr);
                        if (*endptr != '\0') {
                            printf("Error: Invalid float value '%s'.\n", new_value);
                            return false;
                        }
                        *(float*)cell = float_value;
                        break;
                    }
                    case Date: {
                        datetype date_value = parse_date_string(new_value);
                        if(date_value==0){
                            printf("Error: Invalid date value '%s' .\n", new_value);
                            return false;
                        }
                        *(datetype*)cell = date_value;
                        break;
                    }
                } 
            updated++;
            }
        }
    if (updated > 0) {
        printf("%d row(s) updated in %s where %s = %s.\n", updated, table_name, cond_column, cond_value);
    } else {
        printf("No rows found with %s = %s.\n", cond_column, cond_value);
    }
    return true;
}


void query_all_rows(const char *table_name) {
    int ind = find_table(table_name);
    if (ind==-1) {
        printf("Table '%s' not found.\n", table_name);
        return;
    }
    TableMetaData *table  = DB.tables[ind];
    
    printf("Table: %s\n", table->name);
    
    for (int i = 0; i < table->num_cols; i++) {
        printf("%-15s ", table->columns[i]);
    }
    printf("\n");

    for (int i = 0; i < table->num_cols; i++) {
        printf("---------------");
    }
    printf("\n");
    
    if (DB.tables[ind]->num_rows == 0) {
        printf("No rows in table.\n");
        return;
    }
    
    
    for (int i = 0; i < table->num_rows; i++) {

        char *row_data = (char*)table->memory[i];
        
        for (int j = 0; j < table->num_cols; j++) {

            void *cell_location = row_data + table->offset[j];
            
            switch (table->datatypes[j]){
                case Integer: {
                    int value = *(int*)cell_location;
                    printf("%-15d\t", value);
                    break;
                }
                case String: {
                    char *value = (char*)cell_location;
                    printf("%-15s\t", value);
                    break;
                }
                case Character: {
                    char value = *(char*)cell_location;
                    printf("%-15c\t", value);
                    break;
                }
                case Bool: {
                        bool val = *(bool*)cell_location;
                        printf("%-15s\t", val ? "true" : "false");
                        break;
                    }
                    case Float: {
                        float val = *(float*)cell_location;
                        printf("%-15.2f\t", val);
                        break;
                    }
                    case Date: {
                       datetype val = *(datetype*)cell_location;
                        print_date(val);
                        break;
                    }

            }
            
        }
        printf("\n");
    }
    
    printf("\nTotal rows: %d\n", table->num_rows);
}

