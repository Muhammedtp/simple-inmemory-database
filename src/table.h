#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 16
#define MAX_TABLES 16
#define MAX_COLUMNS 16
#define MAX_ROWS 100

//Internal table representation
enum dtype{
    String,
    Integer,
    Character,
};
typedef struct {
    char name[STRING_SIZE];
    int num_rows;
    int num_cols;
    size_t rowsize;
    char columns[MAX_COLUMNS][STRING_SIZE];
    size_t offset[MAX_COLUMNS];
    enum dtype datatypes[MAX_COLUMNS];
    void **memory;
} TableMetaData;

struct DataBase {
    int num_tables;
    TableMetaData *tables[MAX_TABLES];
};

extern struct DataBase DB;

void create_table(char *name, const int num_columns, char *columns[],int datatypes[]);
int find_table(char *name);
void insert(char *table_name, char *values[], const int num_cols);
void query_rows(char *table_name, char *column_name, char *value);
void delete_rows(char *table_name, char *column_name, char *value);
void update_rows(char *table_name, char *cond_column, char *cond_value, char *target_column, char *new_value);
void verify_table(char *name);
void delete_table(char *table_name);
void drop_table(char *table);
void free_database();
int if_match(TableMetaData *table,int col_in,char *value,int row_in);
void query_all_rows(const char *table_name);


#endif