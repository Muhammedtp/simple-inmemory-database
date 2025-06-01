# Simple In-Memory SQL Database Engine

A lightweight, in-memory SQL database engine implemented in C that supports basic relational database operations.

## Features

- **SQL Command Support**: CREATE TABLE, INSERT, SELECT, UPDATE, DELETE, DROP TABLE
- **Interactive Shell**: Command-line interface similar to traditional SQL databases
- **In-Memory Storage**: Fast data operations without disk I/O
- **String-based Data**: Stores values as Strings
- **Memory Management**: Proper cleanup and error handling

## Supported SQL Commands

```sql
-- Create a table
CREATE TABLE students (name, age, rollno);

-- Insert data
INSERT INTO students VALUES ('John', '20', '12345');

-- Query data
SELECT * FROM students;
SELECT * FROM students WHERE age = '20';

-- Update records
UPDATE students SET age = '21' WHERE name = 'John';

-- Delete records
DELETE FROM students WHERE rollno = '12345';

-- Drop table
DROP TABLE students;
```

## Building and Running

### Prerequisites
- GCC compiler


### Compilation
```bash
# Compile all source files
gcc -o mydb main.c parser.c table.c

# Or use the provided Makefile
make
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
├── main.c          # Main program with interactive shell
├── parser.c/h      # SQL command parser and tokenizer
├── table.c/h       # Table management and data operations
└── README.md       # This file
```

## Technical Specifications

- **Maximum Tables**: 16
- **Maximum Columns per Table**: 16  
- **Maximum Rows per Table**: 100
- **String Field Size**: 16 characters
- **Query Length Limit**: 256 characters

## Example Session

```
Welcome to MyDB - Custom SQL Database Engine
Type 'exit' to quit, 'help' for commands

MyDB> CREATE TABLE users (name, email, age);
MyDB> INSERT INTO users VALUES ('Alice', 'alice@email.com', '25');
MyDB> SELECT * FROM users;
Table: users
name            email           age            
Alice           alice@email.com 25             
MyDB> exit
Goodbye!
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/improvement`)
5. Create a Pull Request


## Author

Muhammed Bin Shafeeq T P - muhammedshafeeq2004@gmail.com
