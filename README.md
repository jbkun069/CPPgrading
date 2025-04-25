# Student Grade Management System

This C++ program is designed to manage student grades, providing functionalities to add students, view records, calculate statistics, sort, and filter data. It is a simple yet efficient tool for academic record-keeping.

## Features

- **Add Student**: Input student details including name, number of subjects, and marks.
- **View All Records**: Display all student records stored in the system.
- **Calculate Statistics**: Compute and display summary statistics such as class average, highest and lowest marks, and grade distribution.
- **Sort Records**: Sort and display student records by name or average.
- **Filter Records**: Filter and display records by grade or performance (above or below a certain average).

## Usage

1. **Compile the Program**:
    ```bash
    g++ -o student_grade_manager main.cpp
    ```
2. **Run the Program**:
    ```bash
    ./student_grade_manager
    ```
3. **Interact with the Menu**: Follow the on-screen menu to add students, view records, calculate statistics, sort, and filter data.

## File Structure

- `main.cpp`: Main source file containing program logic and user interface
- `student.h`: Header file defining the Student class and related functions
- `student.cpp`: Implementation of Student class methods
- `utils.h`: Header file for utility functions used throughout the program
- `utils.cpp`: Implementation of utility functions
- `grades.txt`: Stores student records in a plain text format. Each student's data includes their name, marks, average, and grade.

## Dependencies

- **C++ Standard Library**: The program uses standard C++ libraries for:
  - `iostream`: For input/output operations
  - `fstream`: For file handling
  - `vector`: For dynamic arrays to store student data
  - `algorithm`: For sorting and filtering operations
  - `iomanip`: For formatting output
  - `string`: For string manipulation

## Contributing

Contributions to improve this project are welcome! Here's how you can contribute:

1. **Fork the repository**
2. **Create a new branch**: `git checkout -b feature-branch`
3. **Make your changes**
4. **Test your changes** to ensure they work correctly
5. **Submit a pull request** with a clear description of your improvements

Please follow these coding standards:
- Use meaningful variable and function names
- Add comments to explain complex logic
- Follow consistent code formatting

## License

This project is licensed under the MIT License - see the LICENSE file for details.
This allows for free use, modification, and distribution of the code, provided that the original copyright notice and permission notice are included.