#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 16
#define MAX_TABLES 16
#define MAX_COLUMNS 16
#define MAX_ROWS 100

typedef struct {
    char name[STRING_SIZE];
    char **values;
} ColumnMetaData;

typedef struct {
    char name[STRING_SIZE];
    int num_rows;
    int num_cols;
    ColumnMetaData *columns[MAX_COLUMNS];
} TableMetaData;

struct DataBase {
    int num_tables;
    TableMetaData *tables[MAX_TABLES];
};

extern struct DataBase DB;

void create_table(char *name, const int num_columns, const char *columns[]);
int find_table(char *name);
void insert(char *table_name, char *values[], const int num_cols);
void query_rows(char *table_name, char *column_name, char *value);
void delete_rows(char *table_name, char *column_name, char *value);
void update_rows(char *table_name, char *cond_column, char *cond_value, char *target_column, char *new_value);
void verify_table(char *name);
void delete_table(char *table_name);
void free_database();

#endif