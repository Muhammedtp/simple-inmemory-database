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


void create_table(char *name,const int num_columns, const char *columns[]){
    if (!name || strlen(name) >= STRING_SIZE - 1 || num_columns > MAX_COLUMNS || DB.num_tables >= MAX_TABLES) {
        printf("Error: Invalid input (null or too long table name, too many columns, or too many tables).\n");
        return;
    }

    int index = free_table_index();
    if(index == -1){
        printf("Error: Maximum number of Tables created.\n");
        return;
    }

    DB.tables[index] = (TableMetaData*) malloc(sizeof(TableMetaData));
    if(DB.tables[index]==NULL){
        return;
    }

    strcpy(DB.tables[index]->name,name);
    DB.tables[index]->num_rows = 0;
    DB.tables[index]->num_cols = num_columns;
    for(int i=0;i<MAX_COLUMNS;i++){
        DB.tables[index]->columns[i] = NULL;
    }

    for(int i=0;i<num_columns;i++){
        if (!columns[i] || strlen(columns[i]) >= STRING_SIZE - 1) {
            printf("Error: Invalid column name at index %d (null or too long).\n", i);
            free(DB.tables[index]);
            return;
        }

        DB.tables[index]->columns[i] = (ColumnMetaData *) malloc(sizeof(ColumnMetaData));
        if(DB.tables[index]->columns[i]==NULL){
            free(DB.tables[index]);
            return;
        }

        strcpy(DB.tables[index]->columns[i]->name,columns[i]);
        DB.tables[index]->columns[i]->values = (char **)malloc(MAX_ROWS * sizeof(char *));;
        if (!DB.tables[index]->columns[i]->values) {
            printf("Error: Memory allocation failed for column values.\n");
            for (int j = 0; j <= i; j++) {
                free(DB.tables[index]->columns[j]);
            }
            free(DB.tables[index]);
            return;
        }
        for (int j = 0; j < MAX_ROWS; j++) {
            DB.tables[index]->columns[i]->values[j] = NULL; 
        }
    }
    DB.num_tables++;
    return ;
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

void insert(char *table_name, char *values[], const int num_cols){
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
        printf("Error: Number of values (%d) does not match number of columns (%d).\n", num_cols, DB.tables[ind]->num_cols);
        return;
    }
    if (DB.tables[ind]->num_rows >= MAX_ROWS) {
        printf("Error: Maximum number of rows exceeded.\n");
        return;
    }
    int rno = DB.tables[ind]->num_rows;
    for (int i = 0; i < num_cols; i++) {  
        if (!values[i] || strlen(values[i]) >= STRING_SIZE - 1) {
            printf("Error: Invalid value at index %d (null or too long).\n", i);
            return;
        }
        DB.tables[ind]->columns[i]->values[rno] = (char *)malloc(STRING_SIZE * sizeof(char));
        if (!DB.tables[ind]->columns[i]->values[rno]) {
            printf("Error: Memory allocation failed for row value.\n");
            return;
        }
        strcpy(DB.tables[ind]->columns[i]->values[rno], values[i]);
    }
    DB.tables[ind]->num_rows++;
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
    int col_index = -1;
    for (int i = 0; i < DB.tables[ind]->num_cols; i++) {
        if (strcmp(DB.tables[ind]->columns[i]->name, column_name) == 0) {
            col_index = i;
            break;
        }
    }
    if (col_index == -1) {
        printf("Error: Column '%s' not found.\n", column_name);
        return;
    }
    int found = 0;
    printf("Table: %s\n",table_name);
    for(int i=0;i<DB.tables[ind]->num_cols;i++){
        printf("%-15s\t",DB.tables[ind]->columns[i]->name);
    }
    printf("\n");
    for (int r = 0; r < DB.tables[ind]->num_rows; r++) {
        if (DB.tables[ind]->columns[col_index]->values[r] &&
            strcmp(DB.tables[ind]->columns[col_index]->values[r], value) == 0) {
            found = 1;
            for (int c = 0; c < DB.tables[ind]->num_cols; c++) {
                printf("%-15s\t", DB.tables[ind]->columns[c]->values[r] ? DB.tables[ind]->columns[c]->values[r] : "NULL");
                
            }
            printf("\n");
        }
    }
    if (!found) {
        printf("No rows found with %s = %s.\n", column_name, value);
    }
}

void verify_table(char *name) {
    if (!name) {
        printf("Error: Null table name.\n");
        return;
    }
    int i = find_table(name);
    if (i == -1) {
        printf("Error: Table '%s' not found.\n", name);
        return;
    }
    printf("\nTable details:\n%s\nnum_rows: %d\nnum_cols: %d\n", DB.tables[i]->name, DB.tables[i]->num_rows, DB.tables[i]->num_cols);
    for (int j = 0; j < DB.tables[i]->num_cols; j++) {
        printf("  Column %d: %s", j, DB.tables[i]->columns[j]->name);
        if (DB.tables[i]->num_rows > 0) {
            printf(" (values: ");
            for (int k = 0; k < DB.tables[i]->num_rows; k++) {
                printf("%s", DB.tables[i]->columns[j]->values[k] ? DB.tables[i]->columns[j]->values[k] : "NULL");
                if (k < DB.tables[i]->num_rows - 1) printf(", ");
            }
            printf(")");
        }
        printf("\n");
    }
}

void drop_table(char *table){
    int ind = find_table(table);
    if(ind==-1){
        printf("Error: Table does not exists\n");
        return;
    }
    for (int j = 0; j < DB.tables[ind]->num_cols; j++) {
            if (DB.tables[ind]->columns[j]->values) {
                for (int k = 0; k < MAX_ROWS; k++) {
                    free(DB.tables[ind]->columns[j]->values[k]);
                }
                free(DB.tables[ind]->columns[j]->values);
            }
            free(DB.tables[ind]->columns[j]);
        }
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
    for (int i = 0; i < DB.tables[ind]->num_cols; i++) {
        if (strcmp(DB.tables[ind]->columns[i]->name, column_name) == 0) {
            col_index = i;
            break;
        }
    }
    if (col_index == -1) {
        printf("Error: Column '%s' not found.\n", column_name);
        return;
    }
    int deleted = 0;

    for (int r = DB.tables[ind]->num_rows - 1; r >= 0; r--) {
        if (DB.tables[ind]->columns[col_index]->values[r] &&
            strcmp(DB.tables[ind]->columns[col_index]->values[r], value) == 0) {
        
            for (int c = 0; c < DB.tables[ind]->num_cols; c++) {
                free(DB.tables[ind]->columns[c]->values[r]);
        
                for (int k = r; k < DB.tables[ind]->num_rows - 1; k++) {
                    DB.tables[ind]->columns[c]->values[k] = DB.tables[ind]->columns[c]->values[k + 1];
                }
                DB.tables[ind]->columns[c]->values[DB.tables[ind]->num_rows - 1] = NULL;
            }
            DB.tables[ind]->num_rows--;
            deleted++;
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
    int cond_col_index = -1, target_col_index = -1;
    for (int i = 0; i < DB.tables[ind]->num_cols; i++) {
        if (strcmp(DB.tables[ind]->columns[i]->name, cond_column) == 0) {
            cond_col_index = i;
        }
        if (strcmp(DB.tables[ind]->columns[i]->name, target_column) == 0) {
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
        if (DB.tables[ind]->columns[cond_col_index]->values[r] &&
            strcmp(DB.tables[ind]->columns[cond_col_index]->values[r], cond_value) == 0) {
            free(DB.tables[ind]->columns[target_col_index]->values[r]);
            DB.tables[ind]->columns[target_col_index]->values[r] = (char *)malloc(STRING_SIZE * sizeof(char));
            if (!DB.tables[ind]->columns[target_col_index]->values[r]) {
                printf("Error: Memory allocation failed for new value.\n");
                return;
            }
            strncpy(DB.tables[ind]->columns[target_col_index]->values[r], new_value, STRING_SIZE - 1);
            DB.tables[ind]->columns[target_col_index]->values[r][STRING_SIZE - 1] = '\0';
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
    
    printf("Table: %s\n", DB.tables[ind]->name);
    
    for (int i = 0; i < DB.tables[ind]->num_cols; i++) {
        printf("%-15s ", DB.tables[ind]->columns[i]->name);
    }
    printf("\n");
    
    if (DB.tables[ind]->num_rows == 0) {
        printf("No rows in table.\n");
        return;
    }
    
    for (int i = 0; i < DB.tables[ind]->num_rows; i++) {
        for (int j = 0; j < DB.tables[ind]->num_cols ; j++) {
            printf("%-15s ", DB.tables[ind]->columns[j]->values[i] ? DB.tables[ind]->columns[j]->values[i] : "NULL");
        }
        printf("\n");
    }
}

