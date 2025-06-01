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

// #include "parser.h"

// int main() {
//     printf("=== MyDB Test Suite ===\n\n");
    
//     printf("1. Creating student table...\n");
//     parse_sql("CREATE TABLE student (name, age, rollno);");
    
//     printf("\n2. Inserting test data...\n");
//     parse_sql("INSERT INTO student VALUES ('John', '20', '12345');");
//     parse_sql("INSERT INTO student VALUES ('John1', '20', '12346');");
//     parse_sql("INSERT INTO student VALUES ('Alice', '22', '12347');");
//     parse_sql("INSERT INTO student VALUES ('Bob', '21', '12348');");
    
//     printf("\n3. Displaying all students...\n");
//     parse_sql("SELECT * FROM student;");
    
//     printf("\n4. Finding students with age = 20...\n");
//     parse_sql("SELECT * FROM student WHERE age = '20';");
    
//     printf("\n5. Updating John1's age to 30...\n");
//     parse_sql("UPDATE student SET age = '30' WHERE name = 'John1';");
    
//     printf("\n6. Finding students with age = 30...\n");
//     parse_sql("SELECT * FROM student WHERE age = '30';");
    
//     printf("\n7. Finding student named Alice...\n");
//     parse_sql("SELECT * FROM student WHERE name = 'Alice';");
    
//     printf("\n8. Final state - all students...\n");
//     parse_sql("SELECT * FROM student;");
    
//     printf("\n9. Testing DELETE operation...\n");
//     parse_sql("DELETE FROM student WHERE name = 'Bob';");
    
//     printf("\n10. Final state after delete...\n");
//     parse_sql("SELECT * FROM student;");
    
//     printf("\n11. Creating a second table for testing...\n");
//     parse_sql("CREATE TABLE teacher (name, subject, salary);");
//     parse_sql("INSERT INTO teacher VALUES ('Dr. Smith', 'Math', '50000');");
//     parse_sql("INSERT INTO teacher VALUES ('Ms. Johnson', 'English', '45000');");
    
//     printf("\n12. Showing all teachers...\n");
//     parse_sql("SELECT * FROM teacher;");
    
//     printf("\n13. Dropping the teacher table...\n");
//     parse_sql("DROP TABLE teacher;");
    
//     printf("\n14. Trying to select from dropped table (should fail)...\n");
//     parse_sql("SELECT * FROM teacher;");
    
//     printf("\n15. Student table should still exist...\n");
//     parse_sql("SELECT * FROM student;");
    
//     printf("\n16. Finally dropping student table...\n");
//     parse_sql("DROP TABLE student;");
    
//     printf("\n17. Trying to select from dropped student table (should fail)...\n");
//     parse_sql("SELECT * FROM student;");
    
//     printf("\n=== Test Complete ===\n");
//     free_database();
//     return 0;
// }