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
// CREATE TABLE employees (id INT, name STRING, department CHAR, salary FLOAT, is_manager BOOL, hire_date DATE);
// CREATE TABLE products (product_id INT, product_name STRING, category CHAR, price FLOAT, in_stock BOOL, launch_date DATE);
// CREATE TABLE students (student_id INT, full_name STRING, grade CHAR, gpa FLOAT, enrolled BOOL, enrollment_date DATE);

// INSERT INTO employees VALUES (1, 'John Doe', 'E', 75000.50, true, '15/01/2020');
// INSERT INTO employees VALUES (2, 'Jane Smith', 'M', 85000.75, false, '20-03-2019');
// INSERT INTO employees VALUES (3, 'Bob Wilson', 'S', 65000.00, 1, '10/12/2021');
// INSERT INTO employees VALUES (4, 'Alice Brown', 'E', 70000.25, 0, '05-08-2018');

// INSERT INTO products VALUES (101, 'Gaming Laptop', 'E', 1299.99, true, '01/06/2023');
// INSERT INTO products VALUES (102, 'Office Chair', 'F', 299.50, false, '15-09-2022');
// INSERT INTO products VALUES (103, 'Wireless Mouse', 'E', 49.99, 1, '20/11/2023');

// INSERT INTO students VALUES (1001, 'Emma Johnson', 'A', 3.85, true, '01/09/2023');
// INSERT INTO students VALUES (1002, 'Michael Davis', 'B', 3.20, false, '15-08-2023');

// SELECT * FROM employees;
// SELECT * FROM employees WHERE id = 1;
// SELECT * FROM employees WHERE name = 'John Doe';
// SELECT * FROM employees WHERE department = 'E';
// SELECT * FROM employees WHERE salary = 75000.50;
// SELECT * FROM employees WHERE is_manager = true;
// SELECT * FROM employees WHERE is_manager = false;
// SELECT * FROM employees WHERE is_manager = 1;
// SELECT * FROM employees WHERE hire_date = '15/01/2020';

// SELECT * FROM products WHERE category = 'E';
// SELECT * FROM products WHERE price = 1299.99;
// SELECT * FROM products WHERE in_stock = true;
// SELECT * FROM products WHERE launch_date = '01/06/2023';

// SELECT * FROM students WHERE grade = 'A';
// SELECT * FROM students WHERE gpa = 3.85;
// SELECT * FROM students WHERE enrolled = false;

// UPDATE employees SET salary = 78000.00 WHERE id = 1;
// UPDATE employees SET name = 'John Smith' WHERE id = 1;
// UPDATE employees SET department = 'M' WHERE name = 'Bob Wilson';
// UPDATE employees SET is_manager = true WHERE id = 3;
// UPDATE employees SET hire_date = '10/12/2020' WHERE id = 3;

// UPDATE products SET price = 1199.99 WHERE product_id = 101;
// UPDATE products SET product_name = 'Gaming Laptop Pro' WHERE product_id = 101;
// UPDATE products SET category = 'G' WHERE product_name = 'Gaming Laptop Pro';
// UPDATE products SET in_stock = false WHERE product_id = 102;
// UPDATE products SET launch_date = '01/07/2023' WHERE product_id = 101;

// UPDATE students SET gpa = 3.90 WHERE student_id = 1001;
// UPDATE students SET grade = 'A' WHERE student_id = 1002;
// UPDATE students SET enrolled = true WHERE full_name = 'Michael Davis';

// DELETE FROM employees WHERE id = 4;
// DELETE FROM employees WHERE name = 'Jane Smith';
// DELETE FROM employees WHERE department = 'S';
// DELETE FROM employees WHERE salary = 65000.00;
// DELETE FROM employees WHERE is_manager = false;
// DELETE FROM employees WHERE hire_date = '05-08-2018';

// DELETE FROM products WHERE product_id = 102;
// DELETE FROM products WHERE category = 'F';
// DELETE FROM products WHERE price = 49.99;
// DELETE FROM products WHERE in_stock = false;

// DELETE FROM students WHERE grade = 'B';
// DELETE FROM students WHERE enrolled = false;

// INSERT INTO employees VALUES (5, 'Test User', 'T', 50000.00, TRUE, '01/01/2024');
// INSERT INTO employees VALUES (6, 'Test User2', 'T', 50000.00, FALSE, '01/01/2024');
// INSERT INTO employees VALUES (7, 'Test User3', 'T', 50000.00, yes, '01/01/2024');
// INSERT INTO employees VALUES (8, 'Test User4', 'T', 50000.00, no, '01/01/2024');

// SELECT * FROM employees WHERE is_manager = TRUE;
// SELECT * FROM employees WHERE is_manager = FALSE;
// SELECT * FROM employees WHERE is_manager = yes;
// SELECT * FROM employees WHERE is_manager = no;

// INSERT INTO employees VALUES (9, 'Date Test', 'D', 40000.00, true, '25-12-2023');
// INSERT INTO employees VALUES (10, 'Date Test2', 'D', 40000.00, false, '31/12/2023');
// SELECT * FROM employees WHERE hire_date = '25-12-2023';
// SELECT * FROM employees WHERE hire_date = '31/12/2023';

// DROP TABLE students;
// DROP TABLE products;
// DROP TABLE employees;