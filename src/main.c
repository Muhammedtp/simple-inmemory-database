#include "parser.h"

int main() {
    char query[MAX_QUERY];
    
    printf("Welcome to MyDB - Custom SQL Database Engine\n");
    printf("Type 'exit' to quit, 'help' for commands\n");
    
    while (1) {
        printf("\nMyDB> ");
        fflush(stdout); 
        
        if (fgets(query, sizeof(query), stdin) == NULL) {
            printf("\nExiting...\n");
            break;
        }
        

        size_t len = strlen(query);
        if (len > 0 && query[len-1] == '\n') {
            query[len-1] = '\0';
        }
        
    
        if (strlen(query) == 0) {
            continue;
        }
        
        if (strcmp(query, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        
        if (strcmp(query, "help") == 0) {
            printf("Supported SQL Commands:\n");
            printf("  CREATE TABLE table_name (col1, col2, col3);\n");
            printf("  INSERT INTO table_name VALUES ('val1', 'val2', 'val3');\n");
            printf("  SELECT * FROM table_name WHERE condition_column = 'value';\n");
            printf("  UPDATE table_name SET target_column = 'value' WHERE condition_column = 'value';\n");
            printf("  DELETE FROM table_name WHERE column = 'value';\n");
            printf("  exit - quit the program\n");
            continue;
        }
        
        // Parse and execute SQL command
        parse_sql(query);
    }
    
    printf("Cleaning up database...\n");
    free_database();
    return 0;
 }


// Sample Queries
// CREATE TABLE students (name String, age Integer, grade Character);
// INSERT INTO students VALUES ('Alice', '20', 'A');
// INSERT INTO students VALUES ('Bob', '22', 'B');
// INSERT INTO students VALUES ('Charlie', '19', 'A');
// SELECT * FROM students;
// SELECT * FROM students WHERE grade = 'A';
// UPDATE students SET age = '21' WHERE grade = 'A';
// DELETE FROM students WHERE age = '21';
// DROP TABLE students;