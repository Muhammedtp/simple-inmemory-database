# Simple In-Memory SQL Database Engine

A lightweight, in-memory SQL database engine implemented in C that supports basic relational database operations.

## Features

- **SQL Command Support**: CREATE TABLE, INSERT, SELECT, UPDATE, DELETE, DROP TABLE
- **Interactive Shell**: Command-line interface similar to traditional SQL databases
- **In-Memory Storage**: Fast data operations without disk I/O
- **Support Multiple Data Types**: Can store values as String, Integer, Character, Float, Boolean and Date. 
- **Memory Management**: Proper cleanup and error handling

## Supported SQL Commands

check the sql_rules file as well

```sql
-- Create a table
CREATE TABLE employees (id INT, name STRING, department CHAR, salary FLOAT, is_manager BOOL, hire_date DATE);

-- Insert data
INSERT INTO employees VALUES (1, 'John Doe', 'E', 75000.50, true, '15/01/2020')

-- Query data
SELECT * FROM employees;
SELECT * FROM employees WHERE id = 1
SELECT * FROM employees WHERE hire_date = '15/01/2020'

-- Update records
UPDATE employees SET salary = 78000.00 WHERE id = 1

-- Delete records
DELETE FROM employees WHERE name = 'John Doe';

-- Drop table
DROP TABLE employee
```

## Building and Running

### Prerequisites
- GCC compiler


### Compilation
```bash
# Compile all source files
gcc -o mydb main.c parser.c table.c date.c

```

### Usage
```bash
# Run the interactive shell
./mydb

# You'll see the prompt:
MyDB> 

# Type SQL commands or 'help' for assistance
# Type 'exit' to quit
```

## Project Structure

```
src
├── main.c          # Main program with interactive shell
├── parser.c/h      # SQL command parser and tokenizer
├── table.c/h       # Table management and data operations
└── date.c/h
README.md      # This file
sql_rules 

```

## Technical Specifications

- **Maximum Tables**: 64
- **Maximum Columns per Table**: 64  
- **Maximum Rows per Table**: 1000
- **String Field Size**: 32 characters
- **Query Length Limit**: 256 characters
- **Datatypes**: String ,Integer ,Character, Boolean, FLoat, Date.

## Example Session

```
Welcome to MyDB - Custom SQL Database Engine
Type 'exit' to quit, 'help' for commands

MyDB> CREATE TABLE employees (id INT, name STRING, department CHAR, salary FLOAT, is_manager BOOL, hire_date DATE);
Table 'employees' created successfully with 6 columns.

MyDB> INSERT INTO employees VALUES (1, 'John Doe', 'E', 75000.50, true, '15/01/2020');
Row inserted successfully into table 'employees'. Total rows: 1

MyDB> SELECT * FROM employees;
Table: employees
id              name            department      salary          is_manager      hire_date       
------------------------------------------------------------------------------------------
1               John Doe        E               75000.50        true            15-01-2020

Total rows: 1

MyDB> exit
Goodbye!
Cleaning up database...
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/improvement`)
5. Create a Pull Request


## Author

Muhammed Bin Shafeeq T P - muhammedshafeeq2004@gmail.com
