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
        if (datatypes[i] != String && datatypes[i] != Integer && datatypes[i] != Character) {
            printf("Error: Unsupported datatype at column %d.\n", i);
            return;
        }
    }
    
    int index = free_table_index();
    if(index == -1) {
        printf("Error: Maximum number of tables reached.\n");
        return;
    }
    
    DB.tables[index] = (TableMetaData*) malloc(sizeof(TableMetaData));
    if(DB.tables[index] == NULL) {
        printf("Error: Memory allocation failed for table metadata.\n");
        return;
    }
    
    strcpy(DB.tables[index]->name, name);
    DB.tables[index]->num_rows = 0;
    DB.tables[index]->num_cols = num_columns;
    
    size_t row_size = 0;
    DB.tables[index]->offset[0] = 0;
    
    for(int i = 0; i < num_columns; i++) {
        DB.tables[index]->datatypes[i] = datatypes[i];
        strcpy(DB.tables[index]->columns[i], columns[i]);
        
        size_t column_size;
        if(datatypes[i] == String) {
            column_size = sizeof(char) * STRING_SIZE;
        }
        else if(datatypes[i] == Integer) {
            column_size = sizeof(int);
        }
        else if(datatypes[i] == Character) {
            column_size = sizeof(char);
        }
        
        row_size += column_size;
        
        if(i < num_columns - 1) {
            DB.tables[index]->offset[i + 1] = DB.tables[index]->offset[i] + column_size;
        }
    }
    
    DB.tables[index]->rowsize = row_size;
    DB.tables[index]->memory = NULL;  
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
    char *row_data = (char*)table->memory[row_in];
    void *cell_data = row_data + table->offset[col_in];
    
    int match = 0;
    
    if (table->datatypes[col_in] == Integer) {
        int cell_value = *(int*)cell_data;
        int search_value = atoi(value);
        match = (cell_value == search_value);
        
    } else if (table->datatypes[col_in]== Character) {
        char cell_value = *(char*)cell_data;
        char search_value = value[0];
        match = (cell_value == search_value);
        
    } else if (table->datatypes[col_in] == String) {
        char *cell_value = (char*)cell_data;
        match = (strcmp(cell_value, value) == 0);
    }
    return match;
}

void insert(char *table_name, char *values[], const int num_cols) {
    if (!table_name || !values) {
        printf("Error: Null table name or values.\n");
        return;
    }
    
    int ind = find_table(table_name);
    if (ind == -1) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }
    
    if (num_cols != DB.tables[ind]->num_cols) {
        printf("Error: Number of values (%d) does not match number of columns (%d).\n",  num_cols, DB.tables[ind]->num_cols);
        return;
    }
    
    if (DB.tables[ind]->num_rows >= MAX_ROWS) {
        printf("Error: Maximum number of rows exceeded.\n");
        return;
    }
    
    for (int i = 0; i < num_cols; i++) {
        if (!values[i]) {
            printf("Error: Null value at column %d.\n", i);
            return;
        }
    }
    
    TableMetaData *table = DB.tables[ind];
    int current_row = table->num_rows;

    
    if (table->memory == NULL) {
        table->memory = (void**)malloc(sizeof(void*) * MAX_ROWS);
        if (table->memory == NULL) {
            printf("Error: Failed to allocate memory for row pointers.\n");
            return;
        }
    }
    
    table->memory[current_row] = malloc(table->rowsize);
    if (table->memory[current_row] == NULL) {
        printf("Error: Failed to allocate memory for row data.\n");
        return;
    }
    
    char *row_data = (char*)table->memory[current_row];
    
    for (int i = 0; i < num_cols; i++) {
        void *cell_location = row_data + table->offset[i];
        if (table->datatypes[i] == String) {

            strncpy((char*)cell_location, values[i], STRING_SIZE - 1);
            ((char*)cell_location)[STRING_SIZE - 1] = '\0'; 
            
        } else if (table->datatypes[i] == Integer) {
            int int_value = atoi(values[i]);
            *(int*)cell_location = int_value;
            
        } else if (table->datatypes[i] == Character) {
            *(char*)cell_location = values[i][0];
        }
    }
    
    table->num_rows++;
    printf("Row inserted successfully into table '%s'. Total rows: %d\n", table_name, table->num_rows);
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
                
                if (table->datatypes[j] == Integer) {
                    int val = *(int*)cell_location;
                    printf("%-15d", val);
                } else if (table->datatypes[j] == Character) {
                    char val = *(char*)cell_location;
                    printf("%-15c", val);
                } else if (table->datatypes[j] == String) {
                    char *val = (char*)cell_location;
                    printf("%-15s", val);
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

// void verify_table(char *name) {
//     if (!name) {
//         printf("Error: Null table name.\n");
//         return;
//     }
//     int i = find_table(name);
//     if (i == -1) {
//         printf("Error: Table '%s' not found.\n", name);
//         return;
//     }
//     printf("\nTable details:\n%s\nnum_rows: %d\nnum_cols: %d\n", DB.tables[i]->name, DB.tables[i]->num_rows, DB.tables[i]->num_cols);
//     for (int j = 0; j < DB.tables[i]->num_cols; j++) {
//         printf("  Column %d: %s", j, DB.tables[i]->columns[j]->name);
//         if (DB.tables[i]->num_rows > 0) {
//             printf(" (values: ");
//             for (int k = 0; k < DB.tables[i]->num_rows; k++) {
//                 printf("%s", DB.tables[i]->columns[j]->values[k] ? DB.tables[i]->columns[j]->values[k] : "NULL");
//                 if (k < DB.tables[i]->num_rows - 1) printf(", ");
//             }
//             printf(")");
//         }
//         printf("\n");
//     }
// }

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

void update_rows(char *table_name, char *cond_column, char *cond_value, char *target_column, char *new_value) {
    if (!table_name || !cond_column || !cond_value || !target_column || !new_value) {
        printf("Error: Null table name, column name, or value.\n");
        return;
    }
    if (strlen(new_value) >= STRING_SIZE - 1) {
        printf("Error: New value too long.\n");
        return;
    }
    int ind = find_table(table_name);
    if (ind == -1) {
        printf("Error: Table '%s' not found.\n", table_name);
        return;
    }
    TableMetaData *table = DB.tables[ind];
    int cond_col_index = -1, target_col_index = -1;
    for (int i = 0; i < DB.tables[ind]->num_cols; i++) {
        if (strcmp(table->columns[i], cond_column) == 0) {
            cond_col_index = i;
        }
        if (strcmp(table->columns[i], target_column) == 0) {
            target_col_index = i;
        }
    }
    if (cond_col_index == -1) {
        printf("Error: Condition column '%s' not found.\n", cond_column);
        return;
    }
    if (target_col_index == -1) {
        printf("Error: Target column '%s' not found.\n", target_column);
        return;
    }
    int updated = 0;
    for (int r = 0; r < DB.tables[ind]->num_rows; r++) {
        if (if_match(table, cond_col_index, cond_value, r)) {
            void *cell = (char*)table->memory[r] + table->offset[target_col_index];
            if (table->datatypes[target_col_index] == Integer) {
                int value = atoi(new_value);
                *(int*)cell = value;
            } else if (table->datatypes[target_col_index] == Character) {
                char value = new_value[0];
                *(char *)cell = value;
            } else if (table->datatypes[target_col_index] == String) {
                char *ptr = (char*)cell;
                strncpy(ptr, new_value, STRING_SIZE - 1);
                ptr[STRING_SIZE - 1] = '\0';
            }
            updated++;
        }
    }
    if (updated > 0) {
        printf("%d row(s) updated in %s where %s = %s.\n", updated, table_name, cond_column, cond_value);
    } else {
        printf("No rows found with %s = %s.\n", cond_column, cond_value);
    }
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
            
            if (table->datatypes[j] == Integer) {
                int value = *(int*)cell_location;
                printf("%-15d", value);
                
            } else if (table->datatypes[j] == Character) {
                char value = *(char*)cell_location;
                printf("%-15c", value);
                
            } else if (table->datatypes[j] == String) {
                char *value = (char*)cell_location;
                printf("%-15s", value);
            }
        }
        printf("\n");
    }
    
    printf("\nTotal rows: %d\n", table->num_rows);
}

